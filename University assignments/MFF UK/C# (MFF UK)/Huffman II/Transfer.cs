using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Linq;

namespace TreeOfComprasions
{
    class Transfer
    {
        Dictionary<byte, int> Characters = new Dictionary<byte, int>();
        int Index = 0;
        public bool Start(string filename)
        {
            if (!Bytes(filename))
            {
                return false;
            }
            if (Characters.Count == 0) return true;
            HufmanTree rootNode = CreateHufmanTree();
            ExportFile(filename, ref rootNode);
            return true;
        }
        public HufmanTree CreateHufmanTree()
        {
            SortedSet<HufmanTree> HufmanTrees = new SortedSet<HufmanTree>();
            HuffmanDictionary(ref HufmanTrees);
            while (HufmanTrees.Count > 1)
            {
                HufmanTree min1 = HufmanTrees.Min;
                HufmanTrees.Remove(min1);
                HufmanTree min2 = HufmanTrees.Min;
                HufmanTrees.Remove(min2);
                HufmanTrees.Add(new HufmanTree(min1, min2, Index));
                Index++;
            }
            return HufmanTrees.Min;
        }
        private bool Bytes(string file)
        {
            try
            {
                using (FileStream source = new FileStream(file, FileMode.Open, FileAccess.Read))
                {
                    int byteAsInt;
                    while ((byteAsInt = source.ReadByte()) != -1)
                    {
                        byte convertedByte = Convert.ToByte(byteAsInt);
                        if (Characters.ContainsKey(convertedByte))
                        {
                            Characters[convertedByte] += 1;
                        }
                        else
                        {
                            Characters.Add(convertedByte, 1);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                if (ex is IOException || ex is FileNotFoundException)
                {
                    return false;
                }
                else
                {
                    throw;
                }
            }
            return true;
        }
        private void HuffmanDictionary(ref SortedSet<HufmanTree> HufmanTrees)
        {
            List<KeyValuePair<byte, int>> Empty = Characters.ToList<KeyValuePair<byte, int>>();
            while (Empty.Count != 0)
            {
                HufmanTrees.Add(new HufmanTree(Empty[0].Key, Empty[0].Value, Index));
                Index++;
                Empty.RemoveAt(0);
            }

        }
        private void ConvertBytes(ref int byteAsInt,ref Dictionary<byte, List<bool>> dict,ref List<bool> buffer,ref BinaryWriter binary, bool completion)
        {
            if (!completion)
            {
                byte convertedByte = Convert.ToByte(byteAsInt);
                List<bool> addToBuffer = dict[convertedByte];
                buffer.AddRange(addToBuffer);
                if (buffer.Count > 8)
                {
                    byte result = 0;
                    byte multip = 1;
                    for (int i = 0; i < 8; i++)
                    {
                        if (buffer[i]) result += multip;
                        multip *= 2;
                    }
                    buffer.RemoveRange(0, 8);
                    binary.Write(result);
                }
            }
            else
            {
                byte result = 0;
                byte multip = 1;
                int minSize = Math.Min(8, buffer.Count);
                for (int i = 0; i < minSize; i++)
                {
                    if (buffer[i]) result += multip;
                    multip *= 2;
                }
                buffer.RemoveRange(0, minSize);
                binary.Write(result);
            }
        }
        public void ExportFile(string inFile, ref HufmanTree root)
        {
            Dictionary<byte, List<bool>> dict = new Dictionary<byte, List<bool>>();
            BinaryWriter binary = new BinaryWriter(File.Open(inFile + ".huff", FileMode.Create));
            binary.Write(new byte[8] { 0x7B, 0x68, 0x75, 0x7C, 0x6D, 0x7D, 0x66, 0x66 });
            root.Prefix(ref binary, ref dict, new List<bool>());
            binary.Write(new byte[8] { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 });
            FileStream fileStream = new FileStream(inFile, FileMode.Open, FileAccess.Read);
            int byteAsInt;
            List<bool> buffer = new List<bool>();
            while ((byteAsInt = fileStream.ReadByte()) != -1)
            {
                ConvertBytes(ref byteAsInt,ref dict,ref buffer,ref binary,false);
            }
            while (buffer.Count > 0)
            {
                ConvertBytes(ref byteAsInt, ref dict, ref buffer, ref binary, true);
            }
            binary.Close();
        }

    }
}
