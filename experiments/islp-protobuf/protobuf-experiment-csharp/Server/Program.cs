﻿using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace ProtoBufTest
{
    class Program
    {
        private const int listenPort = 4050;

        private static void StartListener()
        {
            UdpClient listener = new UdpClient(listenPort);
            IPEndPoint groupEP = new IPEndPoint(IPAddress.Any, listenPort);

            try
            {
                while (true)
                {
                    //Console.WriteLine("Waiting for broadcast");
                    byte[] bytes = listener.Receive(ref groupEP);

                    //Console.WriteLine($"Received broadcast from {groupEP} :");
                    //Console.WriteLine($" {Encoding.ASCII.GetString(bytes, 0, bytes.Length)}");
                    
                    for (int i = 0; i < bytes.Length; i++)
                    {
                      //  Console.Write(String.Format("{0:X2}",  bytes[i]));    
                    }

                    //Console.WriteLine();

                    
                    JoystickState msg = JoystickState.Parser.ParseFrom(bytes);
                    //Console.WriteLine(msg.Orientation.X);
                    //Console.WriteLine(msg.Orientation.Y);
                    Console.Write(".");
                    
                }
            }
            catch (SocketException e)
            {
                Console.WriteLine(e);
            }
            finally
            {
                listener.Close();
            }
        }

        public static void Main()
        {
            StartListener();
        }
    }
}