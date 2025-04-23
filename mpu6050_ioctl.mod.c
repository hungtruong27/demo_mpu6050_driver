#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0xd3fc359e, "i2c_register_driver" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x92997ed8, "_printk" },
	{ 0x8a38c4d4, "device_destroy" },
	{ 0xfa7080dd, "class_unregister" },
	{ 0x7ba9eeeb, "class_destroy" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xfbb2307d, "i2c_transfer" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x28eb5261, "i2c_transfer_buffer_flags" },
	{ 0x1ea3c9d1, "__register_chrdev" },
	{ 0xb64449fe, "class_create" },
	{ 0xa4a61afc, "device_create" },
	{ 0x80df99b0, "i2c_del_driver" },
	{ 0x51a910c0, "arm_copy_to_user" },
	{ 0x7d5dce20, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("i2c:mpu6050");
MODULE_ALIAS("of:N*T*Cinvensense,mpu6050");
MODULE_ALIAS("of:N*T*Cinvensense,mpu6050C*");

MODULE_INFO(srcversion, "73AD7692395D33EED2483E3");
