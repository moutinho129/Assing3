#include <zephyr/kernel.h>          /* for k_msleep() */
#include <zephyr/device.h>          /* for device_is_ready() and device structure */
#include <zephyr/devicetree.h>		/* for DT_NODELABEL() */
#include <zephyr/drivers/gpio.h>    /* for GPIO api*/
#include <zephyr/drivers/pwm.h>		/* For PWM api */
#include <zephyr/sys/printk.h>      /* for printk()*/
#include "../include/task.h"    

/* Return codes */
#define ERR_ABORT -1	/* If device init fails */

/* Set the PWM period. Note that the HW retricts the maximum value that can be used, so  
 * when assigning errors should be checked to try different values.
*/
#define PWM_PERIOD 10000000 /* Value specified in ns */ 
#define PWM_NLEVELS 2 /* Number of PWM duty-cycle levels*/


/* Get node IDs for LED1, Button0, pwm and extled. Note that 
* LED1 is labeld led0 and Button 1 is labeled button0 in DTS file). 
* pwm and exrled are custom and set in Devicetree overlay*/ 
#define LED1_NODE DT_NODELABEL(led1)
#define SW0_NODE DT_NODELABEL(button0)
#define LEDPWM_NODE DT_NODELABEL(pwm_led2)
#define LED4_NODE DT_NODELABEL(extled4)

/* Define a variable of type static struct gpio_callback, which will latter be used to install the callback
*  It defines e.g. which pin triggers the callback and the address of the function */
static struct gpio_callback button_cb_data;

/* Now get the corresponding device pointer, pin number, configuration flags, ... */
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec led4 = GPIO_DT_SPEC_GET(LED4_NODE, gpios);
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW0_NODE, gpios);
static const struct pwm_dt_spec pwm_led = PWM_DT_SPEC_GET(LEDPWM_NODE);


/* Define a callback function. It is like an ISR (and runs in the context of an ISR) */
/* that is called when the button is pressed */
void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	static int div = 1; /* Divider for computing the duty-cycle */
   
    /* Toggle led1 */
	gpio_pin_toggle_dt(&led1);
	gpio_pin_toggle_dt(&led4);

	/* Adjust the brightness of pwm led 
	 * PWM_NLEVELS levels of intensity, which are actually dividers that set the duty-cycle */
	div++;
	if(div > PWM_NLEVELS)
		div = 1;
	
	printk("PWM divider set to %d\n\r", div);
	
	pwm_set_dt(&pwm_led, PWM_PERIOD, PWM_PERIOD/((unsigned int)div)); /* args are period and Ton */

}

/* 
 * The main function
 */
int control_task(void)
{
	int ret;
	
	/* Welcome message */
	printk("PWM + int + Dig out + Dig in demo \n\r");
	printk("Hit button 1,  led 2 toggles and intensity of led3 cycles \n\r");

	/* Check if devices are ready */
	if (!device_is_ready(led1.port)) {
		printk("Error: led1 device %s is not ready\n", led1.port->name);
		return ERR_ABORT;
	}

	if (!device_is_ready(led4.port)) {
		printk("Error: led4 device %s is not ready\n", led4.port->name);
		return ERR_ABORT;
	}

	if (!device_is_ready(button.port)) {
		printk("Error: button device %s is not ready\n", button.port->name);
		return ERR_ABORT;
	}

	if (!device_is_ready(pwm_led.dev)) {
		printk("Error: PWM device %s is not ready\n", pwm_led .dev->name);
		return ERR_ABORT;
	}

	/* Configure the GPIO pins - led for output and button for input
	 * Use internal pull-up to avoid the need for an external resitor (button)
	 */
	ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		printk("Error: gpio_pin_configure_dt failed for led1, error:%d", ret);
		return ERR_ABORT;
	}

	ret = gpio_pin_configure_dt(&led4, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		printk("Error: gpio_pin_configure_dt failed for led4, error:%d", ret);
		return ERR_ABORT;
	}

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT | GPIO_PULL_UP);
	if (ret < 0) {
		printk("Error: gpio_pin_configure_dt failed for button, error:%d", ret);
		return ERR_ABORT;
	}

	/* Configure the interrupt on the button's pin */
	ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE );
	if (ret < 0) {
		printk("Error: gpio_pin_interrupt_configure_dt failed for button, error:%d", ret);
		return ERR_ABORT;
	}

	printk("All devices initialized succesfully!\n\r");

	/* Initialize the static struct gpio_callback variable   */
    gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin)); 	
	
	/* Add the callback function by calling gpio_add_callback()   */
	 gpio_add_callback(button.port, &button_cb_data);

	/* Set the initial pwm period and pulse duration, both in ns*/
	pwm_set_dt(&pwm_led, PWM_PERIOD, PWM_PERIOD/(5U));
	/* 
 	 * No need for a while(1) at the end of the main. 
	 * In Zephyr main() is a thread that can terminate without resetting the system 
	 * and there is an idle() thread that is called when there are not other tasks/ints ready
	 * The idle() thread activates the power saving modes, so it should be left running when possible
 	 */ 	

	return 0;
}
