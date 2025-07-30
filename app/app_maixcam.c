// Copyright (c) 2024 �״׵��ӹ�����

#include "app_maixcam.h"

// Ĭ�ϻص�����
static void default_laser_callback(LaserCoord_t coord)
{

    // ��ӡ��������������
    if (coord.type == RED_LASER_ID)
    {
        // ��һ�ζ�ȡ������ֵ
        app_pid_init();
        // ����Ĭ��Ŀ��λ��
        app_pid_set_target(100, 100);
        // ��ӡ����ֵ
        my_printf(&huart1, "R: X=%d, Y=%d\r\n", coord.x, coord.y);
        // ����PID
        app_pid_start();
    }

    else
        my_printf(&huart1, "G: X=%d, Y=%d\r\n", coord.x, coord.y);
}

// �ص�����ָ�룬Ĭ��ָ��Ĭ�ϻص�����
static LaserCoordCallback_t laser_coord_callback = default_laser_callback;

// �����������ݻص�����
void maixcam_set_callback(LaserCoordCallback_t callback)
{
    if (callback != NULL)
        laser_coord_callback = callback;
    else
        laser_coord_callback = default_laser_callback;
}

// MaixCam ���ݽ���������������ʽ��$����,X����,Y����,У���\n
int maixcam_parse_data(char *buffer)
{
    if (!buffer || buffer[0] != FRAME_HEADER)
        return -1; // ֡ͷ����

    // �ҵ����һ�����ŵ�λ�ã���֮����У���
    char *last_comma = strrchr(buffer, ',');
    if (!last_comma)
        return -2; // ���ݸ�ʽ�����Ҳ���У��ͷָ���

    // ����У���
    int calc_checksum = 0;
    char *p = buffer;
    while (p < last_comma)
    {
        calc_checksum += (unsigned char)*p;
        p++;
    }
    calc_checksum &= 0xFF; // ȡ��8λ

    // �������յ���У���
    int actual_checksum = 0;
    if (sscanf(last_comma + 1, "%X", &actual_checksum) != 1)
        return -3; // У��͸�ʽ����

    if (calc_checksum != actual_checksum)
        return -4; // У��ʹ���

    // ������������
    LaserCoord_t coord;
    int parsed = sscanf(buffer, "$%c,%d,%d", &coord.type, &coord.x, &coord.y);

    if (parsed != 3)
        return -5; // ����ʧ��

    // ��鼤������
    if (coord.type != RED_LASER_ID && coord.type != GREEN_LASER_ID)
        return -6; // δ֪�ļ�������

    // ͨ���ص���������������
    if (laser_coord_callback)
        laser_coord_callback(coord);

    return 0; // �ɹ�
}

// MaixCam ���ݽ���������
void maixcam_task(MultiTimer *timer, void *userData)
{
    int length_cam = rt_ringbuffer_data_len(&ringbuffer_cam);
    if (length_cam > 0)
    {
        rt_ringbuffer_get(&ringbuffer_cam, output_buffer_cam, length_cam);
        output_buffer_cam[length_cam] = '\0';
        int result = maixcam_parse_data((char *)output_buffer_cam);
        memset(output_buffer_cam, 0, length_cam);
    }
    
//    multiTimerStart(&mt_pid, 10, app_pid_task, NULL);
}
