using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WordCountingC
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length == 1)
            {
                Check(args[0]);
            }
            else
            {
                Console.WriteLine("Argument Error");
                return;
            }
        }
        static void Check(string args)
        {
            StreamReader Input;
            try
            {
                Input = new StreamReader(args);
                int Count = Counter(Input);
                Console.WriteLine(Count);
            }
            catch
            {
                Console.WriteLine("File Error");
                return;
            }

        }
        static int Counter(StreamReader Input)
        {
            int Count = 0;
            char[] del = { ' ', '\n', '\t' };
            bool OnLine;
            string Line = Input.ReadLine();
            while (Line != null)
            {
                OnLine = true;
                for (int i = 0; i < Line.Length; ++i)
                {
                    if (OnLine == true)
                    {
                        if (del.Contains(Line[i]) == false)
                        {
                            OnLine = false;
                        }
                    }
                    else
                    {
                        if (del.Contains(Line[i]) == true)
                        {
                            Count = Count + 1;
                            OnLine = true;
                        }
                    }
                }
                if (OnLine == false)
                {
                    Count = Count + 1;
                }
                Line = Input.ReadLine();
            }
            Input.Close();
            return Count;
        }
    }
}
