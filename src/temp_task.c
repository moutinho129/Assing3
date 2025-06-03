/**
 * @file temp_task.c
 * 
 * Tarefa responsável por ler a temperatura do sensor TC74 via I2C e atualizar a RTDB.
 * A tarefa lê a temperatura a cada segundo, atualizando a RTDB de forma segura com mutex.
 * 
 * @author Nuno Moutinho - 98626 e João Ferreira - 98180
 * @date 2025/06/03
 * @bug Nenhum conhecido
 */
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
#include <stdlib.h>
#include "../include/task.h"



#define TC74_NODE DT_NODELABEL(tc74sensor)
static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(TC74_NODE);

extern struct RTDB db;
extern struct k_mutex db_lock;

void temp_task(void *a, void *b, void *c){
    if (!device_is_ready(dev_i2c.bus)) {
        printk("I2C bus %s is not ready!\n", dev_i2c.bus->name);
        return;
    } else {
        printk("I2C bus %s, device address 0x%02x ready\n", dev_i2c.bus->name, dev_i2c.addr);
    }

    // Envia comando RTR só UMA vez antes do ciclo de leitura!
    uint8_t cmd = 0x00;
    int ret = i2c_write_dt(&dev_i2c, &cmd, 1);
    if(ret != 0){
      //  printk("Failed to write RTR to TC74 (err=%d)\n", ret);
    }

    while(1){
        uint8_t temp_raw = 0;
        ret = i2c_read_dt(&dev_i2c, &temp_raw, 1);
        if(ret != 0){
            //printk("Failed to read temperature from TC74 (err=%d)\n", ret);
            k_sleep(K_MSEC(200));
            continue;
        }

        int8_t temp = (int8_t)temp_raw;

        // Atualiza a RTDB de forma segura
        k_mutex_lock(&db_lock, K_FOREVER);
        db.current_temp = temp;
        k_mutex_unlock(&db_lock);

        //printk("Temperatura lida: %d °C\n", temp);

        k_sleep(K_SECONDS(1));
    }
}
