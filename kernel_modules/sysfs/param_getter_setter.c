#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>
#include<linux/delay.h>

char str_value[4096] = {'\0'};

int set_my_param(const char *val, const struct kernel_param *kp)
{
    
	char *s;
	int len = strlen(val);
	strncpy(str_value, val, len);
	str_value[len-1] = '\0';
	s = strstrip(str_value);
	printk(KERN_INFO "The paramater has been set to %s\n" , s);
	
    return 0;
    
}

 
int get_my_param(char *val, const struct kernel_param *kp)
{
    
	int bytes_written = 0;
	int length = strlen(str_value);
	strncpy(val,str_value, length);
	bytes_written = strlen(val);
	printk(KERN_INFO "Value of paramater being retrieved is %s\n", str_value);
	
    return bytes_written;
    
}

const struct kernel_param_ops my_param_ops = 
{
    
    .set = set_my_param, 
	.get = get_my_param,
    
};
 
module_param_cb(str_value, &my_param_ops, &str_value, 0664 );
 
static int __init hello_world_init(void)
{
    
    printk(KERN_INFO "Kernel Module Inserted Successfully...\n");
    return 0;
    
}
 
void __exit hello_world_exit(void)
{
    printk(KERN_INFO "Kernel Module Removed Successfully...\n");
}
 
module_init(hello_world_init);
module_exit(hello_world_exit);
 
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple string getter setter driver");
MODULE_VERSION("1.0");
