#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/irq.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <mach/hardware.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/input.h>

#define PIN	25

struct input_dev* button_devp;

struct button_irq_desc{
  int irq;
  int pin;
  int number;
  char *name;
 };

static struct button_irq_desc button_irqs[] = {
  {gpio_to_irq(PIN), PIN, 0, "KEY0"},
 };

static irqreturn_t  button_interrupt(int irq, void* dev_id)
{
	struct button_irq_desc *button_irqs = (struct button_irq_desc*)dev_id;
	int down; 
  
	down = gpio_get_value(PIN);

    input_report_key(button_devp, BTN_0, down);
    input_sync(button_devp);
	return IRQ_RETVAL(IRQ_HANDLED);
 }

static int __init button_init(void)
 {
    int i = 0 ;
	int err;
  
	button_devp = input_allocate_device();

	err = request_irq(button_irqs[i].irq, button_interrupt, IRQ_TYPE_EDGE_BOTH, button_irqs[i].name, (void *)&button_irqs[i]);
  
   set_bit(EV_KEY, button_devp->evbit);
   set_bit(BTN_0, button_devp->keybit);

  err = input_register_device(button_devp);
   
   return err;
 }

static void __exit button_exit(void)
{
	int i = 0 ;
	input_unregister_device(button_devp);
	disable_irq(button_irqs[i].irq);
	free_irq(button_irqs[i].irq, (void*)&button_irqs[i]);
 }

module_init(button_init);
module_exit(button_exit);


MODULE_AUTHOR("hubin");
MODULE_LICENSE("GPL");

/*
#include <linux/input.h> 
#include <linux/module.h> 
#include <linux/init.h> 
#include <linux/interrupt.h> 
#include <linux/gpio.h> 

#include <asm/irq.h> 
#include <asm/io.h> 

#define BUTTON_PORT	25
#define BUTTON_IRQ	gpio_to_irq(BUTTON_PORT)

static struct input_dev *button_dev; 


static irqreturn_t button_interrupt(int irq, void *dummy) 
{ 
 input_report_key(button_dev, BTN_0, inb(BUTTON_PORT) & 1); 
 input_sync(button_dev); 
 return IRQ_HANDLED; 
} 


static int __init button_init(void) 
{ 
 int error; 


 if (request_irq(BUTTON_IRQ, button_interrupt, 0, "button", NULL)) { 
    printk(KERN_ERR "button.c: Can't allocate irq %d\n", BUTTON_IRQ ); 
    return -EBUSY; 
	} 


 button_dev = input_allocate_device(); 
 if (!button_dev) { 
 	printk(KERN_ERR "button.c: Not enough memory\n"); 
 	error = -ENOMEM; 
 	goto err_free_irq; 
 } 


 button_dev->evbit[0] = BIT_MASK(EV_KEY); 
 button_dev->keybit[BIT_WORD(BTN_0)] = BIT_MASK(BTN_0); 


 error = input_register_device(button_dev); 
 if (error) { 
 	printk(KERN_ERR "button.c: Failed to register device\n"); 
 	goto err_free_dev; 
 } 


 return 0; 


 err_free_dev: 
 input_free_device(button_dev); 
 err_free_irq: 
 free_irq( BUTTON_IRQ , NULL ); 
 return error; 
} 


static void __exit button_exit(void) 
{ 
	input_unregister_device(button_dev); 
	free_irq(BUTTON_IRQ , NULL); 
} 


module_init(button_init); 
module_exit(button_exit); 
*/
