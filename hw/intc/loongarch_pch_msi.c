/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * QEMU Loongson 7A1000 msi interrupt controller.
 *
 * Copyright (C) 2021 Loongson Technology Corporation Limited
 */

#include "qemu/osdep.h"
#include "hw/sysbus.h"
#include "hw/irq.h"
#include "hw/intc/loongarch_pch_msi.h"
#include "hw/intc/loongarch_pch_pic.h"
#include "hw/pci/msi.h"
#include "hw/misc/unimp.h"
#include "migration/vmstate.h"
#include "trace.h"

static uint64_t loongarch_msi_mem_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void loongarch_msi_mem_write(void *opaque, hwaddr addr,
                                    uint64_t val, unsigned size)
{
    LoongArchPCHMSI *s = LOONGARCH_PCH_MSI(opaque);
    int irq_num = val & 0xff;

    trace_loongarch_msi_set_irq(irq_num);
    qemu_set_irq(s->pch_msi_irq[irq_num - PCH_PIC_IRQ_NUM], 1);
}

static const MemoryRegionOps loongarch_pch_msi_ops = {
    .read  = loongarch_msi_mem_read,
    .write = loongarch_msi_mem_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pch_msi_irq_handler(void *opaque, int irq, int level)
{
    LoongArchPCHMSI *s = LOONGARCH_PCH_MSI(opaque);

    qemu_set_irq(s->pch_msi_irq[irq], level);
}

static void loongarch_pch_msi_init(Object *obj)
{
    LoongArchPCHMSI *s = LOONGARCH_PCH_MSI(obj);
    SysBusDevice *sbd = SYS_BUS_DEVICE(obj);
    int i;

    memory_region_init_io(&s->msi_mmio, obj, &loongarch_pch_msi_ops,
                          s, TYPE_LOONGARCH_PCH_MSI, 0x8);
    sysbus_init_mmio(sbd, &s->msi_mmio);
    msi_nonbroken = true;

    for (i = 0; i < PCH_MSI_IRQ_NUM; i++) {
        sysbus_init_irq(sbd, &s->pch_msi_irq[i]);
    }
    qdev_init_gpio_in(DEVICE(obj), pch_msi_irq_handler, PCH_MSI_IRQ_NUM);
}

static const TypeInfo loongarch_pch_msi_info = {
    .name          = TYPE_LOONGARCH_PCH_MSI,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(LoongArchPCHMSI),
    .instance_init = loongarch_pch_msi_init,
};

static void loongarch_pch_msi_register_types(void)
{
    type_register_static(&loongarch_pch_msi_info);
}

type_init(loongarch_pch_msi_register_types)
