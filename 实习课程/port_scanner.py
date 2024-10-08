import socket
import ipaddress
from concurrent.futures import ThreadPoolExecutor
import argparse

def scan_port(ip, port, timeout=1):
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.settimeout(timeout)
            result = s.connect_ex((str(ip), port))
            if result == 0:
                return ip, port, True
            else:
                return ip, port, False
    except:
        return ip, port, False

def scan_network(network, port_range, num_threads):
    ip_network = ipaddress.ip_network(network)
    ports = range(port_range[0], port_range[1] + 1)
    
    tasks = [(ip, port) for ip in ip_network for port in ports]
    
    results = []
    with ThreadPoolExecutor(max_workers=num_threads) as executor:
        future_results = [executor.submit(scan_port, ip, port) for ip, port in tasks]
        for future in future_results:
            results.append(future.result())
    
    return results

def display_results(results):
    open_ports = [result for result in results if result[2]]
    
    if not open_ports:
        print("没有发现开放的端口。")
    else:
        print("发现的开放端口：")
        for ip, port, _ in open_ports:
            print(f"IP: {ip}, 端口: {port}")
    
    print(f"\n扫描完成。共扫描 {len(results)} 个 IP:端口 组合。")
    print(f"发现 {len(open_ports)} 个开放的端口。")

def main():
    parser = argparse.ArgumentParser(description="网段和端口扫描器")
    parser.add_argument("network", help="要扫描的IP地址或网段 (例如: 192.168.110.0/24)")
    parser.add_argument("--port-range", nargs=2, type=int, default=[1, 1024], 
                        help="要扫描的端口范围 (默认: 1 1024)")
    parser.add_argument("--threads", type=int, default=100, 
                        help="扫描使用的线程数 (默认: 100)")
    
    args = parser.parse_args()
    
    print(f"开始扫描网段 {args.network}，端口范围 {args.port_range[0]}-{args.port_range[1]}，使用 {args.threads} 个线程")
    
    results = scan_network(args.network, args.port_range, args.threads)
    display_results(results)

if __name__ == "__main__":
    main()
