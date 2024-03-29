/***************************************************************************

    idectrl.h

    Generic (PC-style) IDE controller implementation.

    Copyright Nicola Salmoria and the MAME Team.
    Visit http://mamedev.org for licensing and usage restrictions.

***************************************************************************/

#pragma once

#ifndef __IDECTRL_H__
#define __IDECTRL_H__

#include "devlegcy.h"

#include "harddisk.h"


/***************************************************************************
    TYPE DEFINITIONS
***************************************************************************/
typedef struct _ide_hardware ide_hardware;
struct _ide_hardware
{
	void	(*get_info)(device_t *device, UINT8 *buffer, UINT16 &cylinders, UINT8 &sectors, UINT8 &heads);
	int		(*read_sector)(device_t *device, UINT32 lba, void *buffer);
	int		(*write_sector)(device_t *device, UINT32 lba, const void *buffer);
};

typedef struct _ide_config ide_config;
struct _ide_config
{
	void	(*interrupt)(device_t *device, int state);
	const char *master;		/* name of master region (defaults to device tag) */
	const char *slave;		/* name of slave region (defaults to NULL) */
	const char *bmcpu;		/* name of bus master CPU */
	UINT32 bmspace;			/* address space of bus master transfer */
	const ide_hardware *hardware;	/* connected to hardware that is not an hard disk */
};



/***************************************************************************
    DEVICE CONFIGURATION MACROS
***************************************************************************/

#define MCFG_IDE_CONTROLLER_ADD(_tag, _callback) \
	MCFG_DEVICE_ADD(_tag, IDE_CONTROLLER, 0) \
	MCFG_DEVICE_CONFIG_DATAPTR(ide_config, interrupt, _callback)

#define MCFG_IDE_CONTROLLER_REGIONS(_master, _slave) \
	MCFG_DEVICE_CONFIG_DATAPTR(ide_config, master, _master) \
	MCFG_DEVICE_CONFIG_DATAPTR(ide_config, slave,  _slave)

#define MCFG_IDE_BUS_MASTER_SPACE(_cpu, _space) \
	MCFG_DEVICE_CONFIG_DATAPTR(ide_config, bmcpu, _cpu) \
	MCFG_DEVICE_CONFIG_DATA32(ide_config, bmspace, AS_##_space)

#define MCFG_IDE_CONNECTED_TO(_hardware) \
	MCFG_DEVICE_CONFIG_DATAPTR(ide_config, hardware, _hardware) \


/***************************************************************************
    FUNCTION PROTOTYPES
***************************************************************************/

UINT8 *ide_get_features(device_t *device, int drive);

void ide_set_master_password(device_t *device, const UINT8 *password);
void ide_set_user_password(device_t *device, const UINT8 *password);

void ide_set_gnet_readlock(device_t *device, const UINT8 onoff);

int ide_bus_r(device_t *config, int select, int offset);
void ide_bus_w(device_t *config, int select, int offset, int data);

UINT32 ide_controller_r(device_t *config, int reg, int size);
void ide_controller_w(device_t *config, int reg, int size, UINT32 data);

READ32_DEVICE_HANDLER( ide_controller32_r );
WRITE32_DEVICE_HANDLER( ide_controller32_w );
READ32_DEVICE_HANDLER( ide_controller32_pcmcia_r );
WRITE32_DEVICE_HANDLER( ide_controller32_pcmcia_w );
READ32_DEVICE_HANDLER( ide_bus_master32_r );
WRITE32_DEVICE_HANDLER( ide_bus_master32_w );

READ16_DEVICE_HANDLER( ide_controller16_r );
WRITE16_DEVICE_HANDLER( ide_controller16_w );


/* ----- device interface ----- */

DECLARE_LEGACY_DEVICE(IDE_CONTROLLER, ide_controller);


#endif	/* __IDECTRL_H__ */
