#include "CameraThread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <errno.h>

#ifdef __ARM_EABI__
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video.hpp>
#include <opencv2/videoio.hpp>
#endif

CameraThread::CameraThread(QObject *parent) : QThread(parent)
{
    m_run = false;
    m_cnt = 0;

    dev = "/dev/video0";
    width = 1920;
    height = 1080;
}

void CameraThread::open()
{
    QMutexLocker locker(&m_mutex);

    m_cnt++;
    m_run = true;

    if (isRunning())
        return;

    QThread::start();
}

void CameraThread::close()
{
    QMutexLocker locker(&m_mutex);

    m_cnt--;
    m_run = m_cnt != 0;
}

void CameraThread::run()
{
#ifdef __ARM_EABI__
    struct plane_start {
        void * start;
    };

    struct buffer {
        struct plane_start* pstart;
        struct v4l2_plane* planes;
    };

    int fd;
    fd_set fds;
    struct timeval tv;
    int ret = -1, r;
    __u32 i, j;
    __u32 num_planes;
    struct v4l2_capability cap;
    struct v4l2_format fmt;
    struct v4l2_requestbuffers req;
    struct v4l2_buffer buf;
    struct v4l2_plane* planes;
    struct plane_start* pstart;
    struct buffer *buffers;
    enum v4l2_buf_type type;

    fd = ::open(dev.toStdString().data(), O_RDWR);

    if (fd < 0) {
        printf("open device: %s fail\n", dev.toStdString().data());
        goto err;
    }

    if (ioctl(fd, VIDIOC_QUERYCAP, &cap) < 0) {
        printf("Get video capability error!\n");
        goto err1;
    }

    if (!(cap.device_caps & V4L2_CAP_VIDEO_CAPTURE_MPLANE)) {
        printf("Video device not support capture!\n");
        goto err1;
    }

    printf("Support capture!\n");

    memset(&fmt, 0, sizeof(struct v4l2_format));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    fmt.fmt.pix_mp.width = width;
    fmt.fmt.pix_mp.height = height;
    fmt.fmt.pix_mp.pixelformat = V4L2_PIX_FMT_UYVY;
    fmt.fmt.pix_mp.field = V4L2_FIELD_NONE;

    if (ioctl(fd, VIDIOC_S_FMT, &fmt) < 0) {
        printf("Set format fail\n");
        goto err1;
    }

    printf("width = %d\n", fmt.fmt.pix_mp.width);
    printf("height = %d\n", fmt.fmt.pix_mp.height);
    printf("nmplane = %d\n", fmt.fmt.pix_mp.num_planes);

    //memset(&fmt, 0, sizeof(struct v4l2_format));
    //fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    //if (ioctl(fd, VIDIOC_G_FMT, &fmt) < 0) {
    //	printf("Set format fail\n");
    //	goto err;
    //}

    //printf("nmplane = %d\n", fmt.fmt.pix_mp.num_planes);

    req.count = 3;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    req.memory = V4L2_MEMORY_MMAP;
    if (ioctl(fd, VIDIOC_REQBUFS, &req) < 0) {
        printf("Reqbufs fail\n");
        goto err1;
    }

    printf("buffer number: %d\n", req.count);

    num_planes = fmt.fmt.pix_mp.num_planes;

    buffers = (buffer*)malloc(req.count * sizeof(*buffers));

    for(i = 0; i < req.count; i++) {
        memset(&buf, 0, sizeof(buf));
        planes = (v4l2_plane*)calloc(num_planes, sizeof(*planes));
        pstart = (plane_start*)calloc(num_planes, sizeof(*pstart));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.m.planes = planes;
        buf.length = num_planes;
        buf.index = i;
        if (-1 == ioctl (fd, VIDIOC_QUERYBUF, &buf)) {
            printf("Querybuf fail\n");
            req.count = i;
            goto err2;
        }

        (buffers + i)->planes = planes;
        (buffers + i)->pstart = pstart;
        for(j = 0; j < num_planes; j++) {
            printf("plane[%d]: length = %d\n", j, (planes + j)->length);
            printf("plane[%d]: offset = %d\n", j, (planes + j)->m.mem_offset);
            (pstart + j)->start = mmap (NULL /* start anywhere */,
                                    (planes + j)->length,
                                    PROT_READ | PROT_WRITE /* required */,
                                    MAP_SHARED /* recommended */,
                                    fd,
                                    (planes + j)->m.mem_offset);
            if (MAP_FAILED == (pstart +j)->start) {
                printf ("mmap failed\n");
                req.count = i;
                goto unmmap;
            }
        }
    }

    for (i = 0; i < req.count; ++i) {
        memset(&buf, 0, sizeof(buf));

        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.length   	= num_planes;
        buf.index       = i;
        buf.m.planes 	= (buffers + i)->planes;

        if (ioctl (fd, VIDIOC_QBUF, &buf) < 0)
            printf ("VIDIOC_QBUF failed\n");
    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;

    if (ioctl(fd, VIDIOC_STREAMON, &type) < 0)
        printf ("VIDIOC_STREAMON failed\n");

    struct v4l2_plane *tmp_plane;
    tmp_plane = (v4l2_plane*)calloc(num_planes, sizeof(*tmp_plane));

    while (m_run)
    {
        FD_ZERO (&fds);
        FD_SET(fd, &fds);
        tv.tv_sec = 1;
        tv.tv_usec = 0;

        r = select (fd + 1, &fds, NULL, NULL, &tv);

        if (-1 == r)
        {
            if (EINTR == errno)
                continue;
            printf ("select err\n");
        }
        if (0 == r)
        {
            fprintf (stderr, "select timeout\n");
        }
        if (-1 == r || 0 == r) {
            if (!m_run)
                break;
            msleep(100);
            continue;
        }

        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.m.planes = tmp_plane;
        buf.length = num_planes;
        if (ioctl (fd, VIDIOC_DQBUF, &buf) < 0)
            printf("dqbuf fail\n");

        for (j = 0; j < num_planes; j++) {
            //printf("plane[%d] start = %p, bytesused = %d\n", j, ((buffers + buf.index)->pstart + j)->start, (tmp_plane + j)->bytesused);
            process(((buffers + buf.index)->pstart + j)->start, (tmp_plane + j)->bytesused);
            break;
        }

        if (!m_run)
            break;

        if (ioctl (fd, VIDIOC_QBUF, &buf) < 0)
            printf("failture VIDIOC_QBUF\n");
    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    if (ioctl(fd, VIDIOC_STREAMOFF, &type) < 0)
        printf("VIDIOC_STREAMOFF fail\n");

    free(tmp_plane);

    ret = 0;
unmmap:
err2:
    for (i = 0; i < req.count; i++) {
        for (j = 0; j < num_planes; j++) {
            if (MAP_FAILED != ((buffers + i)->pstart + j)->start) {
                if (-1 == munmap(((buffers + i)->pstart + j)->start, ((buffers + i)->planes + j)->length))
                    printf ("munmap error\n");
            }
        }
    }

    for (i = 0; i < req.count; i++) {
        free((buffers + i)->planes);
        free((buffers + i)->pstart);
    }

    free(buffers);

err1:
    ::close(fd);
err:
    Q_UNUSED(ret);
    return;
#endif
}

//#include <QElapsedTimer>
void CameraThread::process(const void *buf, int len)
{
#ifdef __ARM_EABI__
    //static QElapsedTimer etimer;
    //printf("CameraThread::etimer:%lld\n", etimer.restart());

    int size = width * height * 2;
    char data[size];
    memcpy(data, buf, len <= size ? len : size);

    QImage img(width, height, QImage::Format_RGB888);

    cv::Mat matSrc(height, width, CV_8UC2, data);
    cv::Mat matDst(height, width, CV_8UC3, img.bits());
    cv::cvtColor(matSrc, matDst, cv::COLOR_YUV2RGB_UYVY);

    emit imageUpdate(img);
#else
    Q_UNUSED(buf);
    Q_UNUSED(len);
#endif
}
