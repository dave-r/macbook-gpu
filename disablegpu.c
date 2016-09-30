/** 
  * Taken from Arch Wiki
  * https://wiki.archlinux.org/index.php/MacBookPro10,x#Graphics_2
  */
#include <stdio.h>
#include <sys/io.h>

#define GMUX_PORT_SWITCH_DISPLAY	0x10
#define GMUX_PORT_SWITCH_DDC		0x28
#define GMUX_PORT_SWITCH_EXTERNAL	0x40
#define GMUX_PORT_DISCRETE_POWER	0x50
#define GMUX_PORT_VALUE			0xc2
#define GMUX_PORT_READ			0xd0https://wiki.archlinux.org/index.php/MacBookPro10,x#Graphics_2
*/
#define GMUX_PORT_WRITE			0xd4

#define GMUX_IOSTART		0x700

typedef unsigned char u8;

enum discrete_state {STATE_ON, STATE_OFF};
enum gpu_id {IGD, DIS};

static void index_write8(int port, u8 val)
{
	outb(val, GMUX_IOSTART + GMUX_PORT_VALUE);
	outb((port & 0xff), GMUX_IOSTART + GMUX_PORT_WRITE);
}

static u8 index_read8(int port)
{
	u8 val;
	outb((port & 0xff), GMUX_IOSTART + GMUX_PORT_READ);
	val = inb(GMUX_IOSTART + GMUX_PORT_VALUE);

	return val;
}

static void set_discrete_state(enum discrete_state state)
{
	if (state == STATE_ON) {	// switch on dGPU
		index_write8(GMUX_PORT_DISCRETE_POWER, 1);
		index_write8(GMUX_PORT_DISCRETE_POWER, 3);
	} else {			// switch off dGPU
		index_write8(GMUX_PORT_DISCRETE_POWER, 1);
		index_write8(GMUX_PORT_DISCRETE_POWER, 0);
	}
}

static u8 get_discrete_state()
{
	return index_read8(GMUX_PORT_DISCRETE_POWER);
}

static void switchto(enum gpu_id id)
{
	if (id == IGD) {	// switch to iGPU
		index_write8(GMUX_PORT_SWITCH_DDC, 1);
		index_write8(GMUX_PORT_SWITCH_DISPLAY, 2);
		index_write8(GMUX_PORT_SWITCH_EXTERNAL, 2);
	} else {		// switch to dGPU
		index_write8(GMUX_PORT_SWITCH_DDC, 2);
		index_write8(GMUX_PORT_SWITCH_DISPLAY, 3);
		index_write8(GMUX_PORT_SWITCH_EXTERNAL, 3);
	}
}

int main(int argc, char **argv)
{
	// printf("Discrete state: 0x%X\n", get_discrete_state());
	
	if (iopl(3) < 0) {
		perror("No IO permissions");
		return 1;
	}

	printf("Switching off Discrete gpu.\n");
	//switchto(IGD);
	set_discrete_state(STATE_OFF);
	printf("Discrete state: 0x%X\n", get_discrete_state());

	return 0;
}