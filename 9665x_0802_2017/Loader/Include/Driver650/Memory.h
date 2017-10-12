# ifndef _MEMORY_650_H
# define _MEMORY_650_H

    COMMENT  /* Memory Map */ /* # define BOOT_ROM_RAM_BASE_ADDR        0x20000000*/

# define DRAM_BASE_ADDR             (0x80000000)
# define DRAM_EXP_BASE_ADDR         (DRAM_BASE_ADDR + 0x180)
# define DRAM_INT_BASE_ADDR         (DRAM_BASE_ADDR + 0x200)

# define StackSize_KNL              0x1000       /* 4KB */
# define StackSize_INT              0x400        /* 1KB */

# define FAT_HEAP_BUFFER_SIZE       0xA0000
# define LOADER_TMP_BUFFER_SIZE     0x80000

# define BOOT_ROM_SP_LIMIT_ADDR     0xB0800600
# define BOOT_ROM_RAM_TEMP_ADDR     0xB0800200
# define BOOT_ROM_CFG_TEMP_ADDR     0xB0800248
# define BOOT_ROM_RAM_BASE_ADDR     0x80000000
# define AHB_CTRL_REG_BASE_ADDR     0xB0100000
# define USB_OTG_REG_BASE_ADDR      0xB0600000
# define DDR_CTRL_REG_BASE_ADDR     0xB0000000      /* DDR phy base address     */
# define DDR_PHY_REG_BASE_ADDR      0xB0001000

# define CLOCK_GEN_REG_BASE_ADDR    0xB0020000
# define TOP_CTRL_REG_BASE_ADDR     0xB0010000
# define PAD_CTRL_REG_BASE_ADDR     0xB0030000
# define GPIO_CTRL_REG_BASE_ADDR    0xB0070000



# define UART0_REG_BASE_ADDR        0xB0290000
# define SM_HOST_REG_BASE_ADDR      0xB0400000
# define MS_HOST_REG_BASE_ADDR      0xB0410000
# define SD_HOST1_REG_BASE_ADDR     0xB0420000
# define SD_HOST2_REG_BASE_ADDR     0xB0430000
# define SPI_CTRL_REG_BASE_ADDR     0xB0230000
# define IDE_CTRL_REG_BASE_ADDR     0xB0800000
# define TIMER_REG_BASE_ADDR        0xB0040000

# define BOOT_LOADER_TMP_BUFFER     0x80001000
# define CHIP_REMAP_REG_OFFSET      0x00000088
# define CPE_RTC_BASE               0xB0060000      /* RTC controller           */
# define CPE_GPIO_BASE              0xB0070000      /* GPIO controller          */
# define INTERRUPT_BASE_ADDR        0xB0080000      /* Interrupt controller     */


// DDR phy duty configuration register bit
#define DDR_PHY_INCREASE_DUTY       0x00            // P < N
#define DDR_PHY_DECREASE_DUTY       0x10            // P > N
#define DDR_PHY_CLK_ADJ_EN          0x20

// DDR phy duty calibration control register bit
#define DDR_PHY_CLK_OP_ONGOING      0x10
#define DDR_PHY_CLK_OP_DONE         0x00
#define DDR_PHY_DQS_OP_ONGOING      0x20
#define DDR_PHY_DQS_OP_DONE         0x00



# define DDR_PHY_CAL_CLK_ADJ_OFS    0x000           /* 0x00                     */
# define DDR_PHY_CAL_CMD_ADJ_OFS    0x01C           /* 0x1C                     */
# define DDR_PHY_CAL_CTRL_OFS       0x150           /* 0x54                     */
# define DDR_PHY_CAL_CLK_CFG_OFS    0x154           /* 0x55                     */
# define DDR_PHY_CAL_DQS_CFG_OFS    0x158           /* 0x56                     */
# define DDR_PHY_CAL_DTY_CNT_LB_OFS 0x15C           /* 0x57 duty cnt low byte   */
# define DDR_PHY_CAL_DTY_CNT_HB_OFS 0x160           /* 0x58 duty cnt high byte  */
# define DDR_PHY_CAL_DQS0_ADJ_OFS   0x1D0           /* 0x1D0                    */
# define DDR_PHY_CAL_DQ_ADJ_OFS     0x1D4           /* 0x1D0                    */



# define BOOT_SOURCE_NAND_HMG       0x00
# define BOOT_SOURCE_CARD           0x01
# define BOOT_SOURCE_SPI            0x02
# define BOOT_SOURCE_USB_FULL       0x03
# define BOOT_SOURCE_NAND_RS        0x04
# define BOOT_SOURCE_USB            0x05
# define BOOT_SOURCE_DEFAULT        0x07
# define BOOT_FAIL_DISABLE          0x80

# define BOOT_SOURCE_SDIO1          0x00
# define BOOT_SOURCE_MS             0x10
# define BOOT_SOURCE_SM             0x20
# define BOOT_SOURCE_SDIO2          0x30


# define IDENTIFY_ERR       2
# define MBR_ERR            3
# define PBR_ERR            4
# define READ512_ERR        5
# define READ_ERR           7
# define HPA_ERR            8
# define CHECKSUM_ERR       9

    END
# endif
