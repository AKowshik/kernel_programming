#include <linux/module.h>
#include <linux/kernel.h> 
#include <linux/init.h>
#include <linux/mutex.h>

MODULE_LICENSE("GPL");


void list_all_modules(struct list_head head, char* first_mod)
{
    
	struct module *mod = NULL;
    list_for_each_entry(mod, &head, list)
	{
		if(strcmp(mod->name,first_mod) == 0)
		{
            
			return;
		}
		printk(KERN_INFO "Module name : %s", mod->name);
    }
    
}

static int __init init_list_modules(void)
{

	struct module *mod;
	struct list_head list; 
	mutex_lock(&module_mutex);
	char *module_name = "ip_set"; 
	mod = find_module(module_name);

	if(!mod) 
	{
    		printk(KERN_INFO "Could not find module\n");
    		mutex_unlock(&module_mutex);
            
            return -1;
	}

	list = mod->list;
	printk(KERN_INFO "Listing all modules.\nModule name : %s", mod->name);
	list_all_modules(list, module_name);	
	mutex_unlock(&module_mutex);
	
    return 0;

}

static void __exit cleanup_list_modules(void)
{
    
	printk(KERN_INFO "Exitting module\n");
    
}

module_init(init_list_modules);
module_exit(cleanup_list_modules);


