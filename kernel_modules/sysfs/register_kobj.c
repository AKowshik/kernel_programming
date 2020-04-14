#include <linux/module.h> 
#include <linux/printk.h> 
#include <linux/kobject.h> 
#include <linux/sysfs.h> 
#include <linux/init.h> 
#include <linux/fs.h> 
#include <linux/string.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/sched/signal.h>

int num_value = 0;

static int my_set(const char *val, const struct kernel_param *kp)
{

	int n = 0, ret;
	int result;
	ret = kstrtoint(val, 10, &n);
	result =  param_set_int(val, kp);
	num_value = n;
	printk(KERN_INFO "In setter. Value is being set to %d\n",n);
	return result;
}

static int my_get(char *val, const struct kernel_param *kp)
{
	
	int result;
       	result = param_get_int(val, kp);
	printk(KERN_INFO "In getter. Value retrieved is %d\n", result);
	return result;
}
	
static const struct kernel_param_ops param_ops = {
	.set	= my_set,
	.get	= my_get,
};

module_param_cb(num_value, &param_ops, &num_value, 0664);


static struct kobject *example_kobject;
static int foo;

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
        return sprintf(buf, "%d\n", foo);
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
        sscanf(buf, "%du", &foo);
        return count;
}


static struct kobj_attribute foo_attribute =__ATTR(foo, 0660, foo_show, foo_store);

static int __init mymodule_init (void)
{
        int error = 0;

        printk(KERN_INFO "Module initialized successfully \n");

        example_kobject = kobject_create_and_add("my_kobject",
                                                 kernel_kobj);
        if(!example_kobject)
                return -ENOMEM;

        error = sysfs_create_file(example_kobject, &foo_attribute.attr);
        if (error) {
                printk(KERN_INFO "failed to create the foo file in /sys/kernel/my_kobject\n");
        }
	
        return error;
}

static void __exit mymodule_exit (void)
{
        printk(KERN_INFO "Module un initialized successfully \n");
        kobject_put(example_kobject);
}

module_init(mymodule_init);
module_exit(mymodule_exit);

MODULE_LICENSE("GPL");

