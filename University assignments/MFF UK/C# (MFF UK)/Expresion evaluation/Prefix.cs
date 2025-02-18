using System;
using System.Runtime;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

    public class Prefix
    {
        public Prefix(string input)
        {
            FillStack(input);
        }
        Stack<string> values = new Stack<string>();
        bool IsOperand(string op)
        {
            string[] c = new string[] { "+","-","/","*" };
            foreach (string s in c)
            {
                if(op == s)
                {
                    return true;
                }
            }
            return false;
        }
        bool IsNumber(string str)
        {
            foreach (var item in str)
            {
                if (!Char.IsDigit(item))
                {
                    return false;
                }
            }
            return true;
        }
        void FillStack(string input) {
            foreach (var item in input.Split(" ",StringSplitOptions.RemoveEmptyEntries))
            {
                values.Push(item);
            }
        }
        Int32 AddNumber(string nm, bool negative = false)
        {
            Int32 number = 0;
            try
            {
                number = int.Parse(nm);
            }
            catch
            {
                    Console.WriteLine("Format Error");
                    Environment.Exit(0);
            }
            if (negative)
            {
                return number*(-1);
            }
            if(values.Peek() == "~")
            {
                values.Pop();
                return number * (-1);
            }
            return number;
        }
        public Int32 Evaluate()
        {
            Stack<Int32> numbers = new Stack<Int32>();
            while (values.Count > 0)
            {
                if (IsOperand(values.Peek()))
                {
                    numbers.Push(Calculate(values.Pop(),numbers.Pop(),numbers.Pop()));
                    continue;
                }
                Int32 number;
                numbers.Push(AddNumber(values.Pop()));

            }
            if(numbers.Count != 1)
            {
                Console.WriteLine("Format Error");
                Environment.Exit(0);
            }
            return numbers.Pop();
        }
        Int32 Calculate(string s,Int32 n1,Int32 n2)
        {
        try
        {
            switch (s)
            {
                case "+":
                    checked { return n1 + n2; };
                break;
                case "-":

                    checked { return n1 - n2; };
                break;
                case "/":
                    if (n1 == 0 || n2 == 0)
                {
                    Console.WriteLine("Divide Error");
                    Environment.Exit(0);
                }
                return n1 / n2;
                break;
                case "*":
                    checked{ return n1 * n2; }
                break;
                default:
                    Console.WriteLine("Format Error");
                Environment.Exit(0);
                break;
        }
        }
        catch (OverflowException)
        {
            Console.WriteLine("Overflow Error");
            Environment.Exit(0);
        }
            return 0;
        }
}
