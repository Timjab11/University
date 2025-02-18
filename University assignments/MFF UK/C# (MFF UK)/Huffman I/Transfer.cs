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
            rootNode.TransferHufmanTree(true);
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
    }
}
