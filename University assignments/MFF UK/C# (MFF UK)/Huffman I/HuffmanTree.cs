using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace TreeOfComprasions
{
    class HufmanTree : IComparable<HufmanTree>
    {
        public HufmanTree Left { get; set; }
        public HufmanTree Right { get; set; }
        public Byte Bt { get; set; }
        public int Weigth { get; set; }
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
            int result = Weigth - SecondNode.Weigth;
            if (result != 0) { return result; }
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
    }
}
