#include <zephyr/kernel.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(spi2, LOG_LEVEL_DBG);

#define SPI_DEV_NODE DT_NODELABEL(spi2)

static const struct spi_config spi_cfg = {
    .frequency = 1000000,
    .operation = SPI_WORD_SET(8) | SPI_TRANSFER_MSB,
    .slave = 0,
    .cs = NULL,
};

int main(void)
{
    const struct device *spi_dev = DEVICE_DT_GET(SPI_DEV_NODE);

    if (!device_is_ready(spi_dev)) {
        LOG_ERR("SPI device not ready");
        return 1;
    }

    uint8_t tx_buf[4] = { 0xA5, 0x5A, 0x00, 0xFF };
    uint8_t rx_buf[4] = { 0 };

    const struct spi_buf tx_spi_buf = {
        .buf = tx_buf,
        .len = sizeof(tx_buf)
    };
    const struct spi_buf_set tx_set = {
        .buffers = &tx_spi_buf,
        .count = 1
    };

    struct spi_buf rx_spi_buf = {
        .buf = rx_buf,
        .len = sizeof(rx_buf)
    };
    struct spi_buf_set rx_set = {
        .buffers = &rx_spi_buf,
        .count = 1
    };

    while (1) {
        int ret = spi_transceive(spi_dev, &spi_cfg, &tx_set, &rx_set);

        if (ret == 0) {
            if (memcmp(tx_buf, rx_buf, sizeof(tx_buf)) == 0) {
                LOG_INF("SPI loopback success. RX matches TX");
            } else {
                LOG_WRN("SPI loopback mismatch. RX: %02x %02x %02x %02x",
                        rx_buf[0], rx_buf[1], rx_buf[2], rx_buf[3]);
            }
        } else {
            LOG_ERR("SPI transceive failed: %d", ret);
        }


        k_msleep(1000);
    }
}
