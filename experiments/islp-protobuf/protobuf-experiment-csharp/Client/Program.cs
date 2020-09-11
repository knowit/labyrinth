using System;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Text;
using Google.Protobuf;

namespace ConsoleApp1
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");


            
            var testMessage = new TestMessage();
            testMessage.Msg = 3.14f;
            
            using (MemoryStream stream = new MemoryStream())
            {
                // Save the person to a stream
                testMessage.WriteTo(stream);
                byte[] bytes = stream.ToArray();

                for (int i = 0; i < bytes.Length; i++)
                {
                    Console.Write(String.Format("{0:X2}",  bytes[i]));    
                }

                Console.WriteLine();

                UdpClient udpClient = new UdpClient(4049);
                try{
                    udpClient.Connect("192.168.10.196", 4049);

                    udpClient.Send(bytes, bytes.Length);
                
                    udpClient.Close();
                }
                catch (Exception e ) {
                    Console.WriteLine(e.ToString());
                }

                
            }
         
            
        }
    }
}