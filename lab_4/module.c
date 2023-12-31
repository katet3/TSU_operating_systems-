#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("MIT");
MODULE_AUTHOR("katet_3");
MODULE_DESCRIPTION("Tomsk State University Module");

// Члены арифметической прогрессии - версии ядра Linux
static int g_kernel_versions[] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};

static int __init tsu_module_init(void) {
    int i;
    printk(KERN_INFO "Welcome to the Tomsk State University\n");

    // Вывод версий ядра
    printk(KERN_INFO "Linux kernel versions in arithmetic progression:\n");
    for (i = 0; i < 10; ++i) {
        printk(KERN_INFO "%d\n", g_kernel_versions[i]);
    }

    return 0;
}

static void __exit tsu_module_exit(void) {
    printk(KERN_INFO "Tomsk State University forever!\n");
}

module_init(tsu_module_init);
module_exit(tsu_module_exit);