#include <linux/module.h> 
#include <linux/printk.h> 
#include <linux/kobject.h> 
#include <linux/sysfs.h> 
#include <linux/init.h> 
#include <linux/fs.h> 
#include <linux/string.h> 
#include <linux/wait.h>
#include <linux/sched.h>


MODULE_LICENSE("GPL");

static struct kobject *example_kobject;
static int foo;

static DECLARE_WAIT_QUEUE_HEAD(wq);
static int flag = 0;
char str_value[4096] = {'\0'};


static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{

	if(strcmp(str_value, "") == 0)
	{
		printk(KERN_DEBUG "process %i (%s) going to sleep\n", current->pid, current->comm);
        	wait_event(wq, flag != 0);
        	flag = 0;
        	printk(KERN_DEBUG "awoken %i (%s)\n", current->pid, current->comm);
	}
        return sprintf(buf, "%d\n", foo);
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	printk(KERN_DEBUG "process %i (%s) awakening the readers...\n", current->pid, current->comm);
        flag = 1;
        wake_up(&wq);
        sscanf(buf, "%du", &foo);
        strncpy(str_value, buf, strlen(buf));
	return count;
}


static struct kobj_attribute foo_attribute =__ATTR(foo, 0664, foo_show,
                                                   foo_store);

static int __init mymodule_init (void)
{
        int error = 0;

        pr_debug("Module initialized successfully \n");

        example_kobject = kobject_create_and_add("kobject_example",
                                                 kernel_kobj);
        if(!example_kobject)
                return -ENOMEM;

        error = sysfs_create_file(example_kobject, &foo_attribute.attr);
        if (error) {
                pr_debug("failed to create the foo file in /sys/kernel/kobject_example \n");
        }

        return error;
}

static void __exit mymodule_exit (void)
{
        pr_debug ("Module un initialized successfully \n");
        kobject_put(example_kobject);
}


module_init(mymodule_init);
module_exit(mymodule_exit);
