import smtplib
import random
import time
from email.mime.text import MIMEText
from email.header import Header

def generate_random_content():
    subjects = ["问候", "通知", "邀请", "提醒", "反馈"]
    bodies = [
        "希望一切安好。",
        "请查看附件中的重要信息。",
        "诚挚邀请您参加我们的活动。",
        "别忘了今天的重要会议。",
        "感谢您的宝贵反馈。"
    ]
    return random.choice(subjects), random.choice(bodies)

def send_email(sender, auth_code, receiver, subject, body):
    msg = MIMEText(body, 'plain', 'utf-8')
    msg['Subject'] = Header(subject, 'utf-8')
    msg['From'] = sender
    msg['To'] = receiver

    try:
        smtp_server = smtplib.SMTP_SSL('smtp.qq.com', 465)
        smtp_server.login(sender, auth_code)
        smtp_server.send_message(msg)
        smtp_server.quit()
        print(f"邮件发送成功: {subject}")
    except Exception as e:
        print(f"邮件发送失败: {str(e)}")

def main():
    sender = input("请输入发件人邮箱: ")
    auth_code = input("请输入QQ邮箱授权码: ")
    receiver = input("请输入收件人邮箱: ")
    num_emails = int(input("请输入要发送的邮件数量: "))
    interval = int(input("请输入发送间隔(秒): "))

    for i in range(num_emails):
        subject, body = generate_random_content()
        send_email(sender, auth_code, receiver, subject, body)
        if i < num_emails - 1:
            time.sleep(interval)

if __name__ == "__main__":
    main()