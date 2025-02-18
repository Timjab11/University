using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Linq;

namespace TreeOfComprasions
{
    class HufmanTree : IComparable<HufmanTree>
    {
        public HufmanTree Left { get; set; }
        public HufmanTree Right { get; set; }
        public Byte Bt { get; set; }
        public long Weigth { get; set; }
        public int Index { get; set; } 
        public HufmanTree(byte bt, int weigth, int index)
        {
            Bt = bt;
            Weigth = weigth;
            Left = null;
            Right = null;
            Index = index;
        }
        public HufmanTree(HufmanTree left, HufmanTree right, int index)
        {
            Left = left;
            Right = right;
            Weigth = left.Weigth + right.Weigth;
            Bt = 0;
            Index = index;
        }
        public bool IsLeaf()
        {
            if (Left == null && Right == null)
            {
                return true;
            }
            return false;
        }
        public void TransferHufmanTree(bool final)
        {
            if (IsLeaf())
            {
                string output = "*" + Bt + ":" + Weigth + " ";
                if (!final)
                {
                    output += " ";
                }
                Console.Write(output);
                return;
            }
            else
            {
                Console.Write(Weigth + " ");
                Left.TransferHufmanTree(final);
                Right.TransferHufmanTree(final);
            }
        }
        public int CompareTo(HufmanTree SecondNode)
        {
            if (SecondNode == null) { return 1; }

            if (Weigth - SecondNode.Weigth > 0) return 1;
            else if (Weigth - SecondNode.Weigth < 0) return -1;

            if (IsLeaf() && SecondNode.IsLeaf())
            {
                int comparison = Bt - SecondNode.Bt;
                return comparison;
            }
            if (!IsLeaf() && !SecondNode.IsLeaf())
            {
                return Index - SecondNode.Index;
            }
            else if (IsLeaf()) { return -1; }
            return 1;
        }
        public void Prefix(ref BinaryWriter writer, ref Dictionary<byte, List<bool>> dict, List<bool> encoding)
        {
            BitArray array = new BitArray(BitConverter.GetBytes(Weigth));
            array.LeftShift(1);
            BitArray chr = new BitArray(new byte[1] { Bt });

            for (int i = 0; i < 8; i++)
            {
                array.Set(i + 56, chr.Get(i));
            }
            if (IsLeaf())
            {
                bool[] encodingLocal = new bool[encoding.Count];
                encoding.CopyTo(encodingLocal);
                dict.Add(Bt, encodingLocal.ToList<bool>());
                array.Set(0, true);
                byte[] val = new byte[8];
                array.CopyTo(val, 0);
                writer.Write(val);
            }
            else
            {
                array.Set(0, false);
                byte[] val = new byte[8];
                array.CopyTo(val, 0);
                writer.Write(val);
                encoding.Add(false);
                Left.Prefix(ref writer, ref dict, encoding);
                encoding.RemoveAt(encoding.Count - 1);
                encoding.Add(true);
                Right.Prefix(ref writer, ref dict, encoding);
                encoding.RemoveAt(encoding.Count - 1);

            }
        }
    }
}
