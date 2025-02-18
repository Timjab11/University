using System;
using NezarkaBookstore;
using System.Collections.Generic;
using System.Text;
using System.IO;
namespace BookShoppin
{
    internal class Program
    {
        static void Main(string[] args)
        {
            StreamReader sr = new StreamReader(Console.OpenStandardInput(),Console.InputEncoding);
            ModelStore store = ModelStore.LoadFrom(sr);
            if (store == null) {
                Console.WriteLine("Data error.");
                return; 
            }
            WebPage Web = new WebPage();
            string line;
            while ((line = sr.ReadLine()) != null && line.Length > 0)
            {
                ModelStore.ExecuteCommands(line, store, Web);
            }
            sr.Close();
            Web.WriteOut();
        }
    }
}