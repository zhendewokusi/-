from socket import socket

serv = socket()
serv.bind(('127.0.0.1', 9999))
serv.listen(1)

while True:
    print('等待连接...')
    tcp, addr = serv.accept()
    print(f'[+] {addr} 已连接')
    while True:
        data = input('> ').strip()  # 去除前后空格
        
        if not data:
            print("请输入有效命令。")
            continue
        
        print(f"发送命令: '{data}'")  # 调试输出，查看发送的命令
        tcp.send(data.encode('utf-8'))

        result = tcp.recv(1024).decode('utf-8')
        if result == '断开连接':
            print(f'{addr} {result}\n')
            break
        print(result)
    tcp.close()
