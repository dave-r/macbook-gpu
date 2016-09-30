# macbook-gpu

1. download apple_set_os.efi
	https://github.com/0xbb/apple_set_os.efi/releases

1. move apple_set_os.efi to 
	/boot/apple_set_os.efi

1. edit /etc/grub.d/40_custom, add :
	```
	/* NOTE : paths/devices/guids are specific to the machine/install
	    Use the values from the menu entry in /boot/efi/EFI/fedora/grub.cfg
	*/
	if [ x$feature_platform_search_hint = xy ]; then
	  search --no-floppy --fs-uuid --set=root --hint-bios=hd0,gpt5 --hint-efi=hd0,gpt5 --hint-baremetal=ahci0,gpt5  12c5e823-fb2f-4c0d-aa89-9add83036cae
	else
	  search --no-floppy --fs-uuid --set=root 12c5e823-fb2f-4c0d-aa89-9add83036cae
	fi
	chainloader (${root})/apple_set_os.efi
	boot
	```

1. update grub config
	```
	sudo grub2-mkconfig -o /boot/efi/EFI/fedora/grub.cfg
	```

1. download gpu switch
	https://github.com/0xbb/gpu-switch

1. use `gpu-switch` to switch between integrated (-i) and discrete (-d)

1. run `disable-dgpu.sh` on boot


### Useful commands
```bash
cat /sys/kernel/debug/vgaswitcheroo/switch
```
Show which gpus are on and active

```bash
echo OFF > /sys/kernel/debug/vgaswitcheroo/switch
```
Turn off the currently disabled gpu

```bash
echo ON > /sys/kernel/debug/vgaswitcheroo/switch
```
Turn on the currently disabled gpu