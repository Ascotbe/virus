# !/usr/bin/python
# -*- coding: UTF-8 -*-

import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.application import MIMEApplication
from email.mime.text import MIMEText
from email.header import Header
from email.mime.image import MIMEImage
import zipfile
import cv2
import time
image_list=[]#图片列表
# 第三方 SMTP 服务
mail_host = "smtp.163.com"  # 设置服务器
mail_user = "XXXXX@163.com"  # 用户名
mail_pass = "XXXXXXXXXXXXX"  # 口令
receiver = 'XXXXX@163.com'  #接收邮件
def SendEamil(zip_file,image_file):
    # 邮件内容
    email_box = MIMEMultipart()#创建容器
    # message = MIMEText("警戒警戒！莎莎检测到有人入侵！数据以保存喵~", 'plain', 'utf-8')#邮箱文字
    # email_box.attach(message)#存入
    email_box['From'] =  "Salsa<"+mail_user+ ">"#发送人
    email_box ['To'] =  receiver#发给谁
    email_box ['Subject'] = Header("入侵警报", 'utf-8')#标题

    #发送压缩文件
    zip_apart = MIMEApplication(open(zip_file, 'rb').read())
    zip_apart.add_header('Content-Disposition', 'attachment', filename=zip_file)
    email_box.attach(zip_apart)
    #添加表情包图片

    msgAlternative = MIMEMultipart('alternative')
    email_box.attach(msgAlternative)

    mail_msg = """
    <p>警戒警戒！莎莎检测到有人入侵！数据以保存喵~</p>
    <p><img src="cid:dns_config"></p>
    """
    msgAlternative.attach(MIMEText(mail_msg, 'html', 'utf-8'))
    # 指定图片为当前目录
    file = open(image_file, "rb")
    img_data = file.read()
    file.close()
    img = MIMEImage(img_data)
    img.add_header('Content-ID', 'dns_config')
    email_box.attach(img)
    try:
        smtpObj = smtplib.SMTP()
        smtpObj.connect(mail_host, 25)  # 25 为 SMTP 端口号
        smtpObj.login(mail_user, mail_pass)
        smtpObj.sendmail(mail_user, receiver, email_box.as_string())
        print("发送成功")
        smtpObj.quit()
        smtpObj.close()
    except smtplib.SMTPException as e:
        print(e)

def ZIP(time_name):
    zip_file = zipfile.ZipFile(time_name+'.zip', 'w', zipfile.ZIP_DEFLATED)
    # 把zfile整个目录下所有内容，压缩为new.zip文件
    zip_file.write(time_name+'.jpg')
    # 把c.txt文件压缩成一个压缩文件
    # zip_file.write('c.txt',compress_type=zipfile.ZIP_DEFLATED)
    zip_file.close()
    return time_name+'.zip'


def FaceDetection():
    FaceCascadePath = "haarcascade_frontalface.xml"
    FaceCascade = cv2.CascadeClassifier(FaceCascadePath)
    # FaceCascade = cv2.CascadeClassifier(config.FaceCascadePath)

    # 打开视频捕获设备
    video_capture = cv2.VideoCapture(0)

    while True:
        if not video_capture.isOpened():  # 检查它是否被初始化
            time.sleep(5)
            pass

        # 读视频帧
        ret, frame = video_capture.read()  # 如果帧被正确读取，则为True

        # 转为灰度图像
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        # 调用分类器进行检测,如果有人脸返回一个列表
        faces = FaceCascade.detectMultiScale(
            gray,
            scaleFactor=1.1,
            minNeighbors=5,
            minSize=(30, 30),
            # flags=cv2.cv.CV_HAAR_SCALE_IMAGE
        )
        if len(faces) > 0:
            time_name=str(int(time.time()))
            image_list.append(time_name)
            time.sleep(1)
            cv2.imwrite(time_name + '.jpg', frame)  # 存储为图像
            if len(image_list)>4:#如果容器里面图片超过4张发送邮件，防止疯狂发送
                zip_file=ZIP(time_name)
                SendEamil(zip_file, "test.gif")
                image_list.clear()

        # 画矩形框
        #for (x, y, w, h) in faces:
         #   cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

        # 显示视频
        #cv2.imshow('Video', frame)

        #if cv2.waitKey(1) & 0xFF == ord('q'):
         #   break

    # 关闭摄像头设备
    video_capture.release()

    # 关闭所有窗口
    #cv2.destroyAllWindows()


if __name__ == '__main__':
    FaceDetection()
