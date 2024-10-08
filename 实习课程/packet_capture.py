from scapy.all import *
from datetime import datetime

def packet_callback(pkt):
    if IP in pkt:
        ip_src = pkt[IP].src
        ip_dst = pkt[IP].dst
        proto = pkt[IP].proto

        if TCP in pkt:
            sport = pkt[TCP].sport
            dport = pkt[TCP].dport
        elif UDP in pkt:
            sport = pkt[UDP].sport
            dport = pkt[UDP].dport
        else:
            sport = "N/A"
            dport = "N/A"

        pkt_len = len(pkt)
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]

        print(f"Time: {timestamp}, Src IP: {ip_src}, Src Port: {sport}, "
              f"Dst IP: {ip_dst}, Dst Port: {dport}, "
              f"Protocol: {proto}, Length: {pkt_len}")

def capture_packets(interface="eth0", port=None):
    if port:
        # 如果指定了端口，则只捕获该端口的数据包
        bpf_filter = f"port {port}"
    else:
        # 否则捕获所有数据包
        bpf_filter = ""

    print(f"开始在接口 {interface} 上捕获数据包...")
    if port:
        print(f"监听端口: {port}")
    else:
        print("监听所有端口")

    sniff(iface=interface, filter=bpf_filter, prn=packet_callback, store=0)

if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description="简单的数据包捕获工具")
    parser.add_argument("-i", "--interface", default="eth0", help="要监听的网络接口")
    parser.add_argument("-p", "--port", type=int, help="要监听的特定端口（可选）")

    args = parser.parse_args()

    capture_packets(interface=args.interface, port=args.port)
