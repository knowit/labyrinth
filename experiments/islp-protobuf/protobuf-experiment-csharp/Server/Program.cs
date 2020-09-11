using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace ProtoBufTest
{
    class Program
    {
        public static void Main()
        {
            int recv;
            byte[] data = new byte[1024];
            IPEndPoint ipep = new IPEndPoint(IPAddress.Any, 4049);

            Socket newsock = new Socket(AddressFamily.InterNetwork,
                SocketType.Dgram, ProtocolType.Udp);

            newsock.Bind(ipep);
            Console.WriteLine("Waiting for a client...");

            IPEndPoint sender = new IPEndPoint(IPAddress.Any, 0);
            EndPoint Remote = (EndPoint) (sender);

            recv = newsock.ReceiveFrom(data, ref Remote);
            while (true)
            {
                Console.WriteLine("Message received from {0}:", Remote.ToString());
                Console.WriteLine(Encoding.ASCII.GetString(data, 0, recv));
            }
        }
    }
}