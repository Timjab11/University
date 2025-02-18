using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Linq;

namespace TreeOfComprasions
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Transfer control = new Transfer();
            if (args.Length != 1)
            {
                Console.WriteLine("Argument Error");
                return;
            }
            if (!control.Start(args[0]))
            {
                Console.WriteLine("File Error");
            }
        }
    }
    }
