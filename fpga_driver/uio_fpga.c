/*
 * Xilinx AXI Performance Monitor
 *
 * Copyright (C) 2013 Xilinx, Inc. All rights reserved.
 *
 * Description:
 * This driver is developed for AXI Performance Monitor IP,
 * designed to monitor AXI4 traffic for performance analysis
 * of AXI bus in the system. Driver maps HW registers and parameters
 * to userspace. Userspace need not clear the interrupt of IP since
 * driver clears the interrupt.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/io.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/uio_driver.h>
#include <linux/dma-mapping.h>
#define DRV_NAME		"xilinfpga_uio"
#define DRV_VERSION		"1.0"
#define UIO_DUMMY_MEMSIZE	4096
#define FPGA_IS_OFFSET (0)
/**
 * struct fpga_param - HW parameters structure
 * @mode: Mode in which APM is working
 */
struct fpga_param {
	u32 isr;
    dma_addr_t weight_phy;
};

/**
 * struct fpga_dev - Global driver structure
 * @info: uio_info structure
 * @param: fpga_param structure
 * @regs: IOmapped base address
 */
struct fpga_dev {
	struct uio_info info;
	struct fpga_param param;
	void __iomem *regs;
};

/**
 * fpga_handler - Interrupt handler for APM
 * @irq: IRQ number
 * @info: Pointer to uio_info structure
 *
 * Return: Always returns IRQ_HANDLED
 */
static irqreturn_t fpga_handler(int irq, struct uio_info *info)
{
	struct fpga_dev *fpga = (struct fpga_dev *)info->priv;
	void *ptr;

	ptr = (unsigned long *)fpga->info.mem[2].addr;
	/* Clear the interrupt and copy the ISR value to userspace */
	fpga->param.isr = readl(fpga->regs + FPGA_IS_OFFSET);
	writel(fpga->param.isr, fpga->regs + FPGA_IS_OFFSET);
	memcpy(ptr, &fpga->param, sizeof(struct fpga_param));

	return IRQ_HANDLED;
}

/**
 * fpga_probe - Driver probe function
 * @pdev: Pointer to the platform_device structure
 *
 * Returns: '0' on success and failure value on error
 */

static int fpga_probe(struct platform_device *pdev)
{
	struct fpga_dev *fpga;
	struct resource *res;
	int irq;
	int ret;
	void *ptr;

	fpga = devm_kzalloc(&pdev->dev, (sizeof(struct fpga_dev)), GFP_KERNEL);
	if (!fpga)
		return -ENOMEM;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	fpga->regs = devm_ioremap_resource(&pdev->dev, res);
	if (!fpga->regs) {
		dev_err(&pdev->dev, "unable to iomap registers\n");
		return -ENOMEM;
	}

	fpga->info.mem[0].name = "ITRI_fpga";
	fpga->info.mem[0].addr = res->start;
	fpga->info.mem[0].size = resource_size(res);
	fpga->info.mem[0].memtype = UIO_MEM_PHYS;

	fpga->info.mem[1].addr = (unsigned long) dma_alloc_coherent(&pdev->dev, UIO_DUMMY_MEMSIZE, fpga->param.weight_phy, GFP_KERNEL);
	ptr = (unsigned long *)fpga->info.mem[1].addr;
	fpga->info.mem[1].size = UIO_DUMMY_MEMSIZE;
	fpga->info.mem[1].memtype = UIO_MEM_LOGICAL;

	fpga->info.name = DRV_NAME;
	fpga->info.version = DRV_VERSION;

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		dev_err(&pdev->dev, "unable to get irq\n");
		ret = irq;
        return 0;
	}

	fpga->info.irq = irq;
	fpga->info.handler = fpga_handler;
	fpga->info.priv = fpga;

	ret = uio_register_device(&pdev->dev, &fpga->info);
	if (ret < 0) {
		dev_err(&pdev->dev, "unable to register to UIO\n");
        return 0;
	}

	platform_set_drvdata(pdev, fpga);

	dev_info(&pdev->dev, "ITRI FPGA\n");

	return 0;
}

/**
 * fpga_remove - Driver remove function
 * @pdev: Pointer to the platform_device structure
 *
 * Return: Always returns '0'
 */
static int fpga_remove(struct platform_device *pdev)
{
	struct fpga_dev *fpga = platform_get_drvdata(pdev);

	uio_unregister_device(&fpga->info);

	return 0;
}

static const struct of_device_id fpga_of_match[] = {
	{ .compatible = "itri,fpga", },
	{ /* end of table*/ }
};

static struct platform_driver fpga_driver = {
	.driver = {
		.name = "itri-fpga",
		.of_match_table = fpga_of_match,
	},
	.probe = fpga_probe,
	.remove = fpga_remove,
};

module_platform_driver(fpga_driver);

MODULE_AUTHOR("ITRI");
MODULE_DESCRIPTION("ITRI FPGA Driver");
MODULE_LICENSE("GPL v2");
