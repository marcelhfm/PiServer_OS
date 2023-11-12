//
// Created by marcel on 12.11.23.
//

#ifndef PISERVER_OS_BT_H
#define PISERVER_OS_BT_H

void bt_reset();

void bt_loadfirmware();

void bt_setbaud();

void bt_setbdaddr();

void bt_getbdaddr(unsigned char *bdaddr);

void bt_init();

unsigned int bt_isReadByteReady();

unsigned char bt_readByte();

unsigned char bt_waitReadByte();

void setLEeventmask(unsigned char mask);

void startActiveScanning();

void stopScanning();

void startActiveAdvertising();

void connect(unsigned char *addr);

void sendACLsubscribe(unsigned int handle);

#endif //PISERVER_OS_BT_H
