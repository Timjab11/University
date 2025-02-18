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
        public WordProcessor(string reader_, string writer_, int MaximumLength_, bool HighlitedText_ = false)
        {
            reader = new StreamReader(reader_);
            writer = new StreamWriter(writer_);
            maximumLength = MaximumLength_;
            HighlitedText = HighlitedText_;
        }
        bool HighlitedText;
        public StreamReader reader;
        public StreamWriter writer;
        int maximumLength;
        int SentenceLength = 0;
        bool newLine = false;
        bool Paragraph = false;
        char Space()
        {
            return HighlitedText ? '.' : ' ';
        }
        StringBuilder word = new StringBuilder();
        List<string> text = new List<string>();
        char[] sep = new char[] { ' ', '\n', '\t', '\r' };
        char c;
        public void CloseText()
        {
            if (word.Length > 0)
            {
                ProcessWord();
            }
            if (text.Count > 0)
            {
                GenerateLine(true);
            }
        }
        bool FitsText(string word)
        {
            if (SentenceLength + word.Length + text.Count <= maximumLength)
            {
                return true;
            }
            return false;
        }
        string Highlited()
        {
            if (HighlitedText) {
                return "<-";
            }
            else
            {
                return "";
            }
        }
        void GenerateLine(bool paragraph = false)
        {
            int Spaces = text.Count - 1;
            if (paragraph)
            {
                if (text.Count > 1)
                {
                    StringBuilder Text = new StringBuilder();
                    Text.Append(text[0]);
                    for (int i = 1; i < text.Count; ++i)
                    {
                        Text.Append(Space());
                        Text.Append(text[i]);
                    }
                    writer.WriteLine(Text + Highlited());
                }
                else if (text.Count == 1)
                {
                    StringBuilder Text = new StringBuilder();
                    Text.Append(text[0]);
                    writer.WriteLine(Text + Highlited());
                }
            }
            else
            {
                if (text.Count == 1)
                {
                    StringBuilder Text = new StringBuilder();
                    Text.Append(text[0]);
                    writer.WriteLine(Text + Highlited());
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
                            Text.Append(Space());
                        }
                        if (bonusSpaces > 0)
                        {
                            Text.Append(Space());
                            bonusSpaces -= 1;
                        }
                        Text.Append(text[i]);
                    }
                    writer.WriteLine(Text + Highlited());
                }
            }
            SentenceLength = 0;
            text.Clear();
        }
        public void ProcessWord()
        {
            if(word.Length > 0)
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
        }
        void IsParagraph()
        {
            if (c == '\n')
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
            foreach (char ch in sep)
            {
                if (ch == c)
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
                if (word.Length > 0)
                {
                    ProcessWord();
                }
                IsParagraph();
            }
            else
            {
                if (Paragraph)
                {
                    Paragraph = false;
                    GenerateLine(true);
                    writer.WriteLine(Highlited());
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
            try
            {
                int str = 0;
                if(!IsDigit(args[args.Length - 1]))
                {
                    Console.WriteLine("Argument Error");
                    return;
                }
                if (int.Parse(args[args.Length - 1]) <= 0)
                {
                    Console.WriteLine("Argument Error");
                    return;
                }
                while (!File.Exists(args[str]) && str < args.Length - 2)
                {
                    str++;
                }
                if (str >= args.Length - 2)
                {
                    return;
                }
                if (!(args[0] == "--highlight-spaces"))
                {
                    if (args.Length < 3)
                    {
                        Console.WriteLine("Argument Error");
                        return;
                    }
                    WordProcessor wordProcessor = new WordProcessor(args[str], args[args.Length - 2], int.Parse(args[args.Length - 1]));
                    while (wordProcessor.ReadChar())
                    {
                        wordProcessor.ProcessChar();
                    }
                    wordProcessor.ProcessWord();
                    for (int i = str + 1; i < args.Length - 2; i++)
                    {
                        if (!File.Exists(args[i])) { continue; }
                        wordProcessor.reader.Close();
                        wordProcessor.reader = new StreamReader(args[i]);
                        try
                        {
                            //Main while loop () - for formating the text
                            while (wordProcessor.ReadChar())
                            {
                                wordProcessor.ProcessChar();
                            }
                            wordProcessor.ProcessWord();
                            //
                        }
                        catch
                        {

                        }
                    }
                    wordProcessor.CloseText();
                    wordProcessor.writer.Close();
                }
                else
                {
                    if (args.Length < 4)
                    {
                        Console.WriteLine("Argument Error");
                        return;
                    }
                    WordProcessor wordProcessor = new WordProcessor(args[str], args[args.Length - 2], int.Parse(args[args.Length - 1]), true);
                    while (wordProcessor.ReadChar())
                    {
                        wordProcessor.ProcessChar();
                    }
                    wordProcessor.ProcessWord();
                    for (int i = str + 1; i < args.Length - 2; i++)
                    {
                        if (!File.Exists(args[i])) { continue; }
                        wordProcessor.reader.Close();
                        wordProcessor.reader = new StreamReader(args[i]);
                        try
                        {
                            //Main while loop () - for formating the text
                            while (wordProcessor.ReadChar())
                            {
                                wordProcessor.ProcessChar();
                            }
                            wordProcessor.ProcessWord();
                            //
                        }
                        catch
                        {

                        }
                    }
                    wordProcessor.CloseText();
                    wordProcessor.writer.Close();
                }
            }
            catch (Exception)
            {
                Console.WriteLine("Argument Error");
                throw;
            }
            }
        }
    }
