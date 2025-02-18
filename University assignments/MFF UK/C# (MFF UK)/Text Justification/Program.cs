using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TextFormatter_MK._2
{
    class WordProcessor
    {
        public WordProcessor(string reader_, string writer_, int MaximumLength_)
        {
            reader = new StreamReader(reader_);
            writer = new StreamWriter(writer_);
            maximumLength = MaximumLength_;
        }
        StreamReader reader;
        StreamWriter writer;
        int maximumLength;
        int wordLength = 0;
        int SentenceLength = 0;
        bool newLine = false;
        bool Paragraph = false;
        StringBuilder word = new StringBuilder();
        List<string> text = new List<string>();
        char[] sep = new char[] { ' ', '\n', '\t', '\r' };
        char c;
        public void CloseText()
        {
            if(word.Length > 0)
            {
                ProcessWord();
            }
            if (text.Count > 0)
            {
                GenerateLine(true);
            }
            writer.Close();
            reader.Close();
        }
        bool FitsText(string word)
        {
            if (SentenceLength + word.Length + text.Count <= maximumLength)
            {
                return true;
            }
            return false;
        }
        void GenerateLine(bool paragraph = false)
        {
            int Spaces = text.Count - 1;
            if (paragraph)
            {
                if(text.Count > 1)
                {
                    StringBuilder Text = new StringBuilder();
                    Text.Append(text[0]);
                    for (int i = 1; i < text.Count; ++i)
                    {
                        Text.Append(' ');
                        Text.Append(text[i]);
                    }
                    writer.WriteLine(Text);
                }
                else if(text.Count == 1)
                {
                    StringBuilder Text = new StringBuilder();
                    Text.Append(text[0]);
                    writer.WriteLine(Text);
                }
            }
            else
            {
                if (text.Count == 1)
                {
                    StringBuilder Text = new StringBuilder();
                    Text.Append(text[0]);
                    writer.WriteLine(Text);
                }
                else 
                {
                    int bonusSpaces = (maximumLength - SentenceLength) % (text.Count - 1);
                    int CurrentSpaces = ((maximumLength - SentenceLength) - bonusSpaces) / (text.Count - 1);
                    StringBuilder Text = new StringBuilder();
                    Text.Append(text[0]);
                    for (int i = 1; i < text.Count; ++i)
                    {
                        for (int o = 0; o < CurrentSpaces; o++)
                        {
                            Text.Append(' ');
                        }
                        if(bonusSpaces > 0)
                        {
                            Text.Append(' ');
                            bonusSpaces -= 1;
                        }
                        Text.Append(text[i]);
                    }
                    writer.WriteLine(Text);
                }
            }
            SentenceLength = 0;
            text.Clear();
        }
        void ProcessWord()
        {
            if (!FitsText(word.ToString()))
            {
                GenerateLine();
            }
            SentenceLength += word.Length;
            text.Add(word.ToString());
            word.Clear();
            newLine = false;
        }
        void IsParagraph()
        {
            if(c == '\n')
            {
                if (newLine)
                {
                    Paragraph = true;
                    return;
                }
                newLine = true;
            }
        }
        public bool IsWhite(char c)
        {
            foreach(char ch in sep)
            {
                if(ch == c)
                {
                    return true;
                }
            }
            return false;
        }
        public bool ReadChar()
        {
            if (reader.EndOfStream)
            {
                return false;
            }
            c = (char)reader.Read();
            return true;
        }
        public void ProcessChar()
        {
            if (IsWhite(c))
            {
                if(word.Length > 0)
                {
                    ProcessWord();
                }
                IsParagraph();
            }
            else
            {
                if(Paragraph)
                {
                    Paragraph = false;
                    GenerateLine(true);
                    writer.WriteLine();
                }
                newLine = false;
                word.Append(c);
            }
        }
    }

    internal class Program
    {
        public static bool IsDigit(string i)
        {
            foreach (char c in i)
            {
                if (!char.IsDigit(c))
                {
                    return false;
                }
            }
            return true;
        }
        static void Main(string[] args)
        {
            if (args.Length == 3)
            {
                try
                {
                    if (int.Parse(args[2]) <= 0 && IsDigit(args[2]))
                    {
                        Console.WriteLine("Argument Error");
                    }
                    WordProcessor wordProcessor = new WordProcessor(args[0], args[1], int.Parse(args[2]));
                    //Main while loop () - for formating the text
                    while (wordProcessor.ReadChar())
                    {
                        wordProcessor.ProcessChar();
                    }
                    wordProcessor.CloseText();
                    //
                    return;
                }
                catch (FormatException)
                {
                    Console.WriteLine("Argument Error");
                    return;
                }
                catch (IOException)
                {
                    Console.WriteLine("File Error");
                }
            }
            else
            {
                Console.WriteLine("Argument Error");
                return;
            }
        }
    }
}
