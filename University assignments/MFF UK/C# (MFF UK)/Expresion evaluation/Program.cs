﻿// See https://aka.ms/new-console-template for more information
using System;

namespace MyApp // Note: actual namespace depends on the project name.
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Prefix prefix = new Prefix(Console.ReadLine());
            Console.WriteLine(prefix.Evaluate());
        }
    }
}