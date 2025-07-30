// Copyright (c) 2024 �״׵��ӹ�����

#ifndef __APP_MAIXCAM_H
#define __APP_MAIXCAM_H

#include "bsp_system.h"

// ����Э�鳣��
#define FRAME_HEADER '$'
#define FRAME_FOOTER '\n'
#define RED_LASER_ID 'R'
#define GREEN_LASER_ID 'G'

// �����������ݽṹ
#ifndef LASERCOORD_T_DEFINED
#define LASERCOORD_T_DEFINED
typedef struct {
    char type;    // ��������: 'R'��ʾ��ɫ���⣬'G'��ʾ��ɫ����
    int x;        // X����
    int y;        // Y����
} LaserCoord_t;
#endif

// ����ص���������
typedef void (*LaserCoordCallback_t)(LaserCoord_t coord);

// MaixCam ������
void maixcam_task(MultiTimer *timer, void *userData);

// ���ݽ�������
int maixcam_parse_data(char *buffer);

// �����������ݻص�����
void maixcam_set_callback(LaserCoordCallback_t callback);

#endif /* __APP_MAIXCAM_H */
