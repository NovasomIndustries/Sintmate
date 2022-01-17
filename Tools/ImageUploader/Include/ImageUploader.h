#ifndef IMAGEUPLOADER_H_INCLUDED
#define IMAGEUPLOADER_H_INCLUDED

#define         SERIAL_DEVICE   "/dev/ttyACM0"
#define         BUFSIZE      1024
extern  int serial_open(char *port);
extern  void serial_close(int serial_port);
extern  int read_and_convert(char *infile);
extern  int serial_send_gfx(int serial_port,char *image,int size_in_bytes,char *rx_packet);
extern  int serial_send_store_digit_command(int serial_port,char *command,char *image,char *rx_packet);
extern  int serial_send_store_button_command(int serial_port,char *command,char *button,char *rx_packet);
extern  int serial_tx_rx_command(int serial_port,char *tx_packet,char *rx_packet);
extern  int serial_rx_command(int serial_port,char *rx_packet);
extern  int serial_tx_rx(int serial_port,char *tx_packet,int tx_len,char *rx_packet,char *item_name);
/*
extern  int serial_tx_rx_button(int serial_port,char *tx_packet,int tx_len,char *rx_packet,char *button_name);
extern  int serial_tx_rx_digit(int serial_port,char *tx_packet,int tx_len,char *rx_packet,char *digit_name);
*/
#endif // IMAGEUPLOADER_H_INCLUDED
