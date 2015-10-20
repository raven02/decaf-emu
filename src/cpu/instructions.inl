INS(add, (rD), (rA, rB), (OE, Rc), (opcd == 31, xo2 == 266), (), USER, "Add",
   (GPR[rD], Rc.on(CR[0][LT,GT,EQ,SO]), OE.on(XER[SO,OV])),
   (GPR[rA], GPR[rB]))
INS(addc, (rD), (rA, rB), (OE, Rc), (opcd == 31, xo2 == 10), (), USER, "Add with Carry", (), ())
INS(adde, (rD), (rA, rB), (OE, Rc), (opcd == 31, xo2 == 138), (), USER, "Add Extended", (), ())
INS(addi, (rD), (rA, simm), (), (opcd == 14), (), USER, "Add Immediate", (), ())
INS(addic, (rD), (rA, simm), (), (opcd == 12), (), USER, "Add Immediate with Carry", (), ())
INS(addicx, (rD), (rA, simm), (), (opcd == 13), (), USER, "Add Immediate with Carry and Record", (), ())
INS(addis, (rD), (rA, simm), (), (opcd == 15), (), USER, "Add Immediate Shifted", (), ())
INS(addme, (rD), (rA), (OE, Rc), (opcd == 31, xo2 == 234, !_16_20), (), USER, "Add to Minus One Extended", (), ())
INS(addze, (rD), (rA), (OE, Rc), (opcd == 31, xo2 == 202, !_16_20), (), USER, "Add to Zero Extended", (), ())
INS(divw, (rD), (rA, rB), (OE, Rc), (opcd == 31, xo2 == 491), (), USER, "Divide Word", (), ())
INS(divwu, (rD), (rA, rB), (OE, Rc), (opcd == 31, xo2 == 459), (), USER, "Divide Word Unsigned", (), ())
INS(mulhw, (rD), (rA, rB), (Rc), (opcd == 31, xo2 == 75, !_21), (), USER, "Multiply High Word", (), ())
INS(mulhwu, (rD), (rA, rB), (Rc), (opcd == 31, xo2 == 11, !_21), (), USER, "Multiply High Word Unsigned", (), ())
INS(mulli, (rD), (rA, simm), (), (opcd == 7), (), USER, "Multiply Low Immediate", (), ())
INS(mullw, (rD), (rA, rB), (OE, Rc), (opcd == 31, xo2 == 235), (), USER, "Multiply Low Word", (), ())
INS(neg, (rD), (rA), (OE, Rc), (opcd == 31, xo2 == 104, !_16_20), (), USER, "Negate", (), ())
INS(subf, (rD), (rA, rB), (OE, Rc), (opcd == 31, xo2 == 40), (), USER, "Subtract From", (), ())
INS(subfc, (rD), (rA, rB), (OE, Rc), (opcd == 31, xo2 == 8), (), USER, "Subtract From with Carry", (), ())
INS(subfe, (rD), (rA, rB), (OE, Rc), (opcd == 31, xo2 == 136), (), USER, "Subtract From Extended", (), ())
INS(subfic, (rD), (rA, simm), (), (opcd == 8), (), USER, "Subtract From Immediate with Carry", (), ())
INS(subfme, (rD), (rA), (OE, Rc), (opcd == 31, xo2 == 232, !_16_20), (), USER, "Subtract From Minus One Extended", (), ())
INS(subfze, (rD), (rA), (OE, Rc), (opcd == 31, xo2 == 200, !_16_20), (), USER, "Subtract From Zero Extended", (), ())

// Integer Compare
INS(cmp, (crfD), (rA, rB), (), (opcd == 31, xo1 == 0, !_9, !_31), (L == 1), USER, "Compare", 
   (CR[crfD]), 
   (GPR[rA], GPR[rB]))
INS(cmpi, (crfD), (rA, simm), (), (opcd == 11, !_9), (L == 1), USER, "Compare Immediate",
   (CR[crfD]), 
   (GPR[rA]))
INS(cmpl, (crfD), (rA, rB), (), (opcd == 31, xo1 == 32, !_9, !_31), (L == 1), USER, "Compare Logical",
   (CR[crfD]),
   (GPR[rA], GPR[rB]))
INS(cmpli, (crfD), (rA, uimm), (), (opcd == 10, !_9), (L == 1), USER, "Compare Logical Immediate",
   (CR[crfD]),
   (GPR[rA]))

// Integer Logical
INS(and_, (rA), (rS, rB), (Rc), (opcd == 31, xo1 == 28), (), USER, "AND", (), ())
INS(andc, (rA), (rS, rB), (Rc), (opcd == 31, xo1 == 60), (), USER, "AND with Complement", (), ())
INS(andi, (rA), (rS, uimm), (), (opcd == 28), (), USER, "AND Immediate", (), ())
INS(andis, (rA), (rS, uimm), (), (opcd == 29), (), USER, "AND Immediate Shifted", (), ())
INS(cntlzw, (rA), (rS), (Rc), (opcd == 31, xo1 == 26, !_16_20), (), USER, "Count Leading ZerOEs Word", (), ())
INS(eqv, (rA), (rS, rB), (Rc), (opcd == 31, xo1 == 284), (), USER, "Equivalent", (), ())
INS(extsb, (rA), (rS), (Rc), (opcd == 31, xo1 == 954, !_16_20), (), USER, "Extend Sign Byte", (), ())
INS(extsh, (rA), (rS), (Rc), (opcd == 31, xo1 == 922, !_16_20), (), USER, "Extend Sign Half Word", (), ())
INS(nand, (rA), (rS, rB), (Rc), (opcd == 31, xo1 == 476), (), USER, "NAND", (), ())
INS(nor, (rA), (rS, rB), (Rc), (opcd == 31, xo1 == 124), (), USER, "NOR", (), ())
INS(or_, (rA), (rS, rB), (Rc), (opcd == 31, xo1 == 444), (), USER, "OR", (), ())
INS(orc, (rA), (rS, rB), (Rc), (opcd == 31, xo1 == 412), (), USER, "OR with Complement", (), ())
INS(ori, (rA), (rS, uimm), (), (opcd == 24), (), USER, "OR Immediate", (), ())
INS(oris, (rA), (rS, uimm), (), (opcd == 25), (), USER, "OR Immediate Shifted", (), ())
INS(xor_, (rA), (rS, rB), (Rc), (opcd == 31, xo1 == 316), (), USER, "XOR", (), ())
INS(xori, (rA), (rS, uimm), (), (opcd == 26), (), USER, "XOR Immediate", (), ())
INS(xoris, (rA), (rS, uimm), (), (opcd == 27), (), USER, "XOR Immediate Shifted", (), ())

// Integer Rotate
INS(rlwimi, (rA), (rS, rA, sh, mb, me), (Rc), (opcd == 20), (), USER, "Rotate Left Word Immediate then Mask Insert",
   (GPR[rA], Rc.on(CR[0][LT,GT,EQ,SO])),
   (GPR[rS], GPR[rA]))
INS(rlwinm, (rA), (rS, sh, mb, me), (Rc), (opcd == 21), (), USER, "Rotate Left Word Immediate then AND with Mask", 
   (GPR[rA], Rc.on(CR[0][LT,GT,EQ,SO])),
   (GPR[rS]))
INS(rlwnm, (rA), (rS, rB, mb, me), (Rc), (opcd == 23), (), USER, "Rotate Left Word then AND with Mask", 
   (GPR[rA], Rc.on(CR[0][LT,GT,EQ,SO])),
   (GPR[rS], GPR[rB]))

// Integer Shift
INS(slw, (rA), (rS, rB), (Rc), (opcd == 31, xo1 == 24), (), USER, "Shift Left Word", 
   (GPR[rA], Rc.on(CR[0][LT,GT,EQ,SO])), 
   (GPR[rS], GPR[rB]))
INS(sraw, (rA), (rS, rB), (Rc), (opcd == 31, xo1 == 792), (), USER, "Shift Right Arithmetic Word",
   (GPR[rA], XER[SO], Rc.on(CR[0][LT,GT,EQ,SO])), 
   (GPR[rS], GPR[rB]))
INS(srawi, (rA), (rS, sh), (Rc), (opcd == 31, xo1 == 824), (), USER, "Shift Right Arithmetic Word Immediate",
   (GPR[rA], XER[SO], Rc.on(CR[0][LT,GT,EQ,SO])),
   (GPR[rS]))
INS(srw, (rA), (rS, rB), (Rc), (opcd == 31, xo1 == 536), (), USER, "Shift Right Word",
   (GPR[rA], Rc.on(CR[0][LT,GT,EQ,SO])),
   (GPR[rS], GPR[rB]))

// Floating-Point Arithmetic
INS(fadd, (frD), (frA, frB), (Rc), (opcd == 63, xo4 == 21, !_21_25), (), USER, "Floating Add", (), ())
INS(fadds, (frD), (frA, frB), (Rc), (opcd == 59, xo4 == 21, !_21_25), (), USER, "Floating Add Single", (), ())
INS(fdiv, (frD), (frA, frB), (Rc), (opcd == 63, xo4 == 18, !_21_25), (), USER, "Floating Divide", (), ())
INS(fdivs, (frD), (frA, frB), (Rc), (opcd == 59, xo4 == 18, !_21_25), (), USER, "Floating Divide Single", (), ())
INS(fmul, (frD), (frA, frC), (Rc), (opcd == 63, xo4 == 25, !_16_20), (), USER, "Floating Multiply", (), ())
INS(fmuls, (frD), (frA, frC), (Rc), (opcd == 59, xo4 == 25, !_16_20), (), USER, "Floating Multiply Single", (), ())
INS(fres, (frD), (frB), (Rc), (opcd == 59, xo4 == 24, !_11_15, !_21_25), (), USER, "Floating Reciprocal Estimate Single", (), ())
INS(frsqrte, (frD), (frB), (Rc), (opcd == 63, xo4 == 26, !_11_15, !_21_25), (), USER, "Floating Reciprocal Square Root Estimate", (), ())
INS(fsub, (frD), (frA, frB), (Rc), (opcd == 63, xo4 == 20, !_21_25), (), USER, "Floating Sub", (), ())
INS(fsubs, (frD), (frA, frB), (Rc), (opcd == 59, xo4 == 20, !_21_25), (), USER, "Floating Sub Single", (), ())
INS(fsel, (frD), (frA, frB, frC), (Rc), (opcd == 63, xo4 == 23), (), USER, "Floating Select", (), ())

// Floating-Point Multiply-Add
INS(fmadd, (frD), (frA, frB, frC), (Rc), (opcd == 63, xo4 == 29), (), USER, "Floating Multiply-Add", (), ())
INS(fmadds, (frD), (frA, frB, frC), (Rc), (opcd == 59, xo4 == 29), (), USER, "Floating Multiply-Add Single", (), ())
INS(fmsub, (frD), (frA, frB, frC), (Rc), (opcd == 63, xo4 == 28), (), USER, "Floating Multiply-Sub", (), ())
INS(fmsubs, (frD), (frA, frB, frC), (Rc), (opcd == 59, xo4 == 28), (), USER, "Floating Multiply-Sub Single", (), ())
INS(fnmadd, (frD), (frA, frB, frC), (Rc), (opcd == 63, xo4 == 31), (), USER, "Floating Negative Multiply-Add", (), ())
INS(fnmadds, (frD), (frA, frB, frC), (Rc), (opcd == 59, xo4 == 31), (), USER, "Floating Negative Multiply-Add Single", (), ())
INS(fnmsub, (frD), (frA, frB, frC), (Rc), (opcd == 63, xo4 == 30), (), USER, "Floating Negative Multiply-Sub", (), ())
INS(fnmsubs, (frD), (frA, frB, frC), (Rc), (opcd == 59, xo4 == 30), (), USER, "Floating Negative Multiply-Sub Single", (), ())

// Floating-Point Rounding and Conversion
INS(fctiw, (frD), (frB), (Rc), (opcd == 63, xo1 == 14, !_11_15), (), USER, "Floating Convert to Integer Word", (), ())
INS(fctiwz, (frD), (frB), (Rc), (opcd == 63, xo1 == 15, !_11_15), (), USER, "Floating Convert to Integer Word with Round toward Zero", (), ())
INS(frsp, (frD), (frB), (Rc), (opcd == 63, xo1 == 12, !_11_15), (), USER, "Floating Round to Single", (), ())

// Floating-Point Compare
INS(fcmpo, (crfD), (frA, frB), (), (opcd == 63, xo1 == 32, !_9_10, !_31), (), USER, "Floating Compare Ordered",
   (CR[crfD][LT,GT,EQ,UN], FPSCR[FPCC, FX, VXSNAN, VXVC]),
   (FPR[rA], FPR[rB]))
INS(fcmpu, (crfD), (frA, frB), (), (opcd == 63, xo1 == 0, !_9_10, !_31), (), USER, "Floating Compare Unordered", 
   (CR[crfD][LT,GT,EQ,UN], FPSCR[FPCC, VXSNAN]), 
   (FPR[rA], FPR[rB]))

// Floating-Point Status and Control Register
INS(mcrfs, (crfD), (crfS), (), (opcd == 63, xo1 == 64, !_9_10, !_14_15, !_16_20, !_31), (), USER, "",
   (CR[crfD],
      crfS.eq(0, FPSCR[FX, OX]),
      crfS.eq(1, FPSCR[UX, ZX, XX, VXSNAN]),
      crfS.eq(2, FPSCR[VXISI, VXIDI, VXZDZ, VXIMZ]),
      crfS.eq(3, FPSCR[VXVC]),
      crfS.eq(5, FPSCR[VXSOFT, VXSQRT, VXCVI])),
   (FPSCR[crfS]))
INS(mffs, (frD), (), (Rc), (opcd == 63, xo1 == 583, !_11_15, !_16_20), (), USER, "", 
   (FPR[rD], Rc.on(CR[1][FX,FEX,VX,OX])), 
   (FPSCR))
INS(mtfsf, (), (fm, frB), (Rc), (opcd == 63, xo1 == 711, !_6, !_15), (), USER, "",
   (FPSCR[FIELDMASK(fm)], Rc.on(CR[1][FX, FEX, VX, OX])),
   (FPR[rB]))
INS(mtfsfi, (crfD), (), (Rc, imm), (opcd == 63, xo1 == 134, !_9_10, !_11_15, !_20), (), USER, "",
   (FPSCR[crfD], Rc.on(CR[1][FX, FEX, VX, OX])),
   ())

// Integer Load
INS(lbz, (rD), (rA, d), (), (opcd == 34), (), USER, "Load Byte and Zero",
   (GPR[rD]),
   (MEM(GPRZ[rA] + EXTS(d), 1, uint8_t)))
INS(lbzu, (rD, rA), (rA, d), (), (opcd == 35), (), USER, "Load Byte and Zero with Update", 
   (GPR[rD], GPR[rA]),
   (MEM(GPR[rA] + EXTS(d), 1, uint8_t)))
INS(lbzx, (rD), (rA, rB), (), (opcd == 31, xo1 == 87, !_31), (), USER, "Load Byte and Zero Indexed",
   (GPR[rD]),
   (MEM(GPRZ[rA] + GPR[rB], 1, uint8_t)))
INS(lbzux, (rD, rA), (rA, rB), (), (opcd == 31, xo1 == 119, !_31), (rA == 0, rA == rD), USER, "Load Byte and Zero with Update Indexed",
   (GPR[rD], GPR[rA]),
   (MEM(GPR[rA] + GPR[rB], 1, uint8_t)))
INS(lha, (rD), (rA, d), (), (opcd == 42), (), USER, "Load Half Word Algebraic", 
   (GPR[rD]), 
   (MEM(GPRZ[rA] + EXTS(d), 2, uint16_t)))
INS(lhau, (rD, rA), (rA, d), (), (opcd == 43), (rA == 0, rA == rD), USER, "Load Half Word Algebraic with Update",
   (GPR[rD], GPR[rA]),
   (MEM(GPR[rA] + EXTS(d), 2, uint16_t)))
INS(lhax, (rD), (rA, rB), (), (opcd == 31, xo1 == 343, !_31), (), USER, "Load Half Word Algebraic Indexed",
   (GPR[rD]),
   (MEM(GPRZ[rA] + GPR[rB], 2, uint16_t)))
INS(lhaux, (rD, rA), (rA, rB), (), (opcd == 31, xo1 == 375, !_31), (rA == 0, rA == rD), USER, "Load Half Word Algebraic with Update Indexed",
   (GPR[rD], GPR[rA]),
   (MEM(GPR[rA] + GPR[rB], 2, uint16_t)))
INS(lhz, (rD), (rA, d), (), (opcd == 40), (), USER, "Load Half Word and Zero",
   (GPR[rD]),
   (MEM(GPRZ[rA] + EXTS(d), 2, uint16_t)))
INS(lhzu, (rD, rA), (rA, d), (), (opcd == 41), (rA == 0, rA == rD), USER, "Load Half Word and Zero with Update",
   (GPR[rD], GPR[rA]),
   (MEM(GPR[rA] + EXTS(d), 2, uint16_t)))
INS(lhzx, (rD), (rA, rB), (), (opcd == 31, xo1 == 279, !_31), (), USER, "Load Half Word and Zero Indexed",
   (GPR[rD]),
   (MEM(GPRZ[rA] + GPR[rB], 2, uint16_t)))
INS(lhzux, (rD, rA), (rA, rB), (), (opcd == 31, xo1 == 311, !_31), (rA == 0, rA == rD), USER, "Load Half Word and Zero with Update Indexed",
   (GPR[rD], GPR[rA]),
   (MEM(GPR[rA] + GPR[rB], 2, uint16_t)))
INS(lwz, (rD), (rA, d), (), (opcd == 32), (), USER, "Load Word and Zero",
   (GPR[rD]),
   (MEM(GPRZ[rA] + EXTS(d), 4, uint32_t)))
INS(lwzu, (rD, rA), (rA, d), (), (opcd == 33), (rA == 0, rA == rD), USER, "Load Word and Zero with Update",
   (GPR[rD], GPR[rA]),
   (MEM(GPR[rA] + EXTS(d), 4, uint32_t)))
INS(lwzx, (rD), (rA, rB), (), (opcd == 31, xo1 == 23, !_31), (), USER, "Load Word and Zero Indexed",
   (GPR[rD]),
   (MEM(GPRZ[rA] + GPR[rB], 4, uint32_t)))
INS(lwzux, (rD, rA), (rA, rB), (), (opcd == 31, xo1 == 55, !_31), (rA == 0, rA == rD), USER, "Load Word and Zero with Update Indexed",
   (GPR[rD], GPR[rA]),
   (MEM(GPR[rA] + GPR[rB], 4, uint32_t)))

// Integer Store
INS(stb, (), (rS, rA, d), (), (opcd == 38), (), USER, "Store Byte", (), ())
INS(stbu, (rA), (rS, rA, d), (), (opcd == 39), (), USER, "Store Byte with Update", (), ())
INS(stbx, (), (rS, rA, rB), (), (opcd == 31, xo1 == 215, !_31), (), USER, "Store Byte Indexed", (), ())
INS(stbux, (rA), (rS, rA, rB), (), (opcd == 31, xo1 == 247, !_31), (), USER, "Store Byte with Update Indexed", (), ())
INS(sth, (), (rS, rA, d), (), (opcd == 44), (), USER, "Store Half Word", (), ())
INS(sthu, (rA), (rS, rA, d), (), (opcd == 45), (), USER, "Store Half Word with Update", (), ())
INS(sthx, (), (rS, rA, rB), (), (opcd == 31, xo1 == 407, !_31), (), USER, "Store Half Word Indexed", (), ())
INS(sthux, (rA), (rS, rA, rB), (), (opcd == 31, xo1 == 439, !_31), (), USER, "Store Half Word with Update Indexed", (), ())
INS(stw, (), (rS, rA, d), (), (opcd == 36), (), USER, "Store Word", (), ())
INS(stwu, (rA), (rS, rA, d), (), (opcd == 37), (), USER, "Store Word with Update", (), ())
INS(stwx, (), (rS, rA, rB), (), (opcd == 31, xo1 == 151, !_31), (), USER, "Store Word Indexed", (), ())
INS(stwux, (rA), (rS, rA, rB), (), (opcd == 31, xo1 == 183, !_31), (), USER, "Store Word with Update Indexed", (), ())

// Integer Load and Store with Byte Reverse
INS(lhbrx, (rD), (rA, rB), (), (opcd == 31, xo1 == 790, !_31), (), USER, "Load Half Word Byte-Reverse Indexed", (), ())
INS(lwbrx, (rD), (rA, rB), (), (opcd == 31, xo1 == 534, !_31), (), USER, "Load Word Byte-Reverse Indexed", (), ())
INS(sthbrx, (), (rS, rA, rB), (), (opcd == 31, xo1 == 918, !_31), (), USER, "Store Half Word Byte-Reverse Indexed", (), ())
INS(stwbrx, (), (rS, rA, rB), (), (opcd == 31, xo1 == 662, !_31), (), USER, "Store Word Byte-Reverse Indexed", (), ())

// Integer Load and Store Multiple
INS(lmw, (rD), (rA, d), (), (opcd == 46), (), USER, "Load Multiple Words", (), ())
INS(stmw, (), (rS, rA, d), (), (opcd == 47), (), USER, "Store Multiple Words", (), ())

// Integer Load and Store String
INS(lswi, (rD), (rA, nb), (), (opcd == 31, xo1 == 597, !_31), (), USER, "Load String Word Immediate", (), ())
INS(lswx, (rD), (rA, rB), (), (opcd == 31, xo1 == 533, !_31), (), USER, "Load String Word Indexed", (), ())
INS(stswi, (), (rS, rA, nb), (), (opcd == 31, xo1 == 725, !_31), (), USER, "Store String Word Immediate", (), ())
INS(stswx, (), (rS, rA, rB), (), (opcd == 31, xo1 == 661, !_31), (), USER, "Store String Word Indexed", (), ())

// Memory Synchronisation
INS(eieio, (), (), (), (opcd == 31, xo1 == 854, !_6_10, !_11_15, !_16_20, !_31), (), SPECIAL, "EnfoRce In-Order Execution of I/O", (), ())
INS(isync, (), (), (), (opcd == 19, xo1 == 150, !_6_10, !_11_15, !_16_20, !_31), (), SPECIAL, "Instruction Synchronise", (), ())
INS(lwarx, (rD), (rA, rB), (), (opcd == 31, xo1 == 20, !_31), (), SPECIAL, "Load Word and Reserve Indexed", (), ())
INS(stwcx, (), (rS, rA, rB), (), (opcd == 31, xo1 == 150, _31), (), SPECIAL, "Store Word Conditional Indexed", (), ())
INS(sync, (), (), (), (opcd == 31, xo1 == 598, !_6_10, !_11_15, !_16_20, !_31), (), SPECIAL, "Synchronise", (), ())

// Floating-Point Load
INS(lfd, (frD), (rA, d), (), (opcd == 50), (), USER, "Load Floating-Point Double", 
   (FPR[rD]), 
   (MEM(GPRZ[rA] + EXTS(d), 1, double)))
INS(lfdu, (frD, rA), (rA, d), (), (opcd == 51), (rA == 0), USER, "Load Floating-Point Double with Update", 
   (FPR[rD], GPR[rA]), 
   (MEM(GPR[rA] + EXTS(d), 1, double)))
INS(lfdx, (frD), (rA, rB), (), (opcd == 31, xo1 == 599, !_31), (), USER, "Load Floating-Point Double Indexed", 
   (FPR[rD]),
   (MEM(GPRZ[rA] + GPR[rB], 1, double)))
INS(lfdux, (frD, rA), (rA, rB), (), (opcd == 31, xo1 == 631, !_31), (rA == 0), USER, "Load Floating-Point Double with Update Indexed", 
   (FPR[rD], GPR[rA]),
   (MEM(GPR[rA] + GPR[rB], 1, double)))
INS(lfs, (frD), (rA, d), (), (opcd == 48), (), USER, "Load Floating-Point Single", 
   (PSR[rD]), 
   (MEM(GPRZ[rA] + EXTS(d), 1, float)))
INS(lfsu, (frD, rA), (rA, d), (), (opcd == 49), (rA == 0), USER, "Load Floating-Point Single with Update",
   (PSR[rD], GPR[rA]),
   (MEM(GPR[rA] + EXTS(d), 1, float)))
INS(lfsx, (frD), (rA, rB), (), (opcd == 31, xo1 == 535, !_31), (), USER, "Load Floating-Point Single Indexed",
   (PSR[rD]),
   (MEM(GPRZ[rA] + GPR[rB], 1, float)))
INS(lfsux, (frD, rA), (rA, rB), (), (opcd == 31, xo1 == 567, !_31), (rA == 0), USER, "Load Floating-Point Single with Update Indexed",
   (PSR[rD], GPR[rA]),
   (MEM(GPR[rA] + GPR[rB], 1, float)))

// Floating-Point Store
INS(stfd, (), (frS, rA, d), (), (opcd == 54), (), USER, "Store Floating-Point Double", (), ())
INS(stfdu, (rA), (frS, rA, d), (), (opcd == 55), (), USER, "Store Floating-Point Double with Update", (), ())
INS(stfdx, (), (frS, rA, rB), (), (opcd == 31, xo1 == 727, !_31), (), USER, "Store Floating-Point Double Indexed", (), ())
INS(stfdux, (rA), (frS, rA, rB), (), (opcd == 31, xo1 == 759, !_31), (), USER, "Store Floating-Point Double with Update Indexed", (), ())
INS(stfiwx, (), (frS, rA, rB), (), (opcd == 31, xo1 == 983, !_31), (), USER, "Store Floating-Point as Integer Word Indexed", (), ())
INS(stfs, (), (frS, rA, d), (), (opcd == 52), (), USER, "Store Floating-Point Single", (), ())
INS(stfsu, (rA), (frS, rA, d), (), (opcd == 53), (), USER, "Store Floating-Point Single with Update", (), ())
INS(stfsx, (), (frS, rA, rB), (), (opcd == 31, xo1 == 663, !_31), (), USER, "Store Floating-Point Single Indexed", (), ())
INS(stfsux, (rA), (frS, rA, rB), (), (opcd == 31, xo1 == 695, !_31), (), USER, "Store Floating-Point Single with Update Indexed", (), ())

// Floating-Point Move
INS(fabs, (frD), (frB), (Rc), (opcd == 63, xo1 == 264, !_11_15), (), USER, "Floating Absolute Value", (), ())
INS(fmr, (frD), (frB), (Rc), (opcd == 63, xo1 == 72, !_11_15), (), USER, "Floating Move Register", (), ())
INS(fnabs, (frD), (frB), (Rc), (opcd == 63, xo1 == 136, !_11_15), (), USER, "Floating Negative Absolute Value", (), ())
INS(fneg, (frD), (frB), (Rc), (opcd == 63, xo1 == 40, !_11_15), (), USER, "Floating Negate", (), ())

// Branch
INS(b, (), (li), (aa, lk), (opcd == 18), (), BRANCH, "Branch", (), ())
INS(bc, (bo), (bi, bd), (aa, lk), (opcd == 16), (), BRANCH, "Branch Conditional", (), ())
INS(bcctr, (bo), (bi), (lk), (opcd == 19, xo1 == 528, !_16_20), (), BRANCH, "Branch Conditional to CTR", (), ())
INS(bclr, (bo), (bi), (lk), (opcd == 19, xo1 == 16, !_16_20), (), BRANCH, "Branch Conditional to LR", (), ())

// Condition Register Logical
INS(crand, (crbD), (crbA, crbB), (), (opcd == 19, xo1 == 257, !_31), (), USER, "Condition Register AND", (), ())
INS(crandc, (crbD), (crbA, crbB), (), (opcd == 19, xo1 == 129, !_31), (), USER, "Condition Register AND with Complement", (), ())
INS(creqv, (crbD), (crbA, crbB), (), (opcd == 19, xo1 == 289, !_31), (), USER, "Condition Register Equivalent", (), ())
INS(crnand, (crbD), (crbA, crbB), (), (opcd == 19, xo1 == 225, !_31), (), USER, "Condition Register NAND", (), ())
INS(crnor, (crbD), (crbA, crbB), (), (opcd == 19, xo1 == 33, !_31), (), USER, "Condition Register NOR", (), ())
INS(cror, (crbD), (crbA, crbB), (), (opcd == 19, xo1 == 449, !_31), (), USER, "Condition Register OR", (), ())
INS(crorc, (crbD), (crbA, crbB), (), (opcd == 19, xo1 == 417, !_31), (), USER, "Condition Register OR with Complement", (), ())
INS(crxor, (crbD), (crbA, crbB), (), (opcd == 19, xo1 == 193, !_31), (), USER, "Condition Register XOR", (), ())
INS(mcrf, (crfD), (crfS), (), (opcd == 19, xo1 == 0, !_9_10, !_14_15, !_16_20, !_31), (), USER, "Move Condition Register Field", (), ())

// System Linkage
INS(rfi, (), (), (), (opcd == 19, xo1 == 50, !_6_10, !_11_15, !_16_20, !_31), (), SUPER, "", (), ())
INS(sc, (), (), (), (opcd == 17, !_6_10, !_11_15, !_16_29, _30, !_31), (), SPECIAL, "Syscall", (), ())
INS(kc, (), (kcn), (), (opcd == 1), (), SPECIAL, "krncall", (), ())

// Trap
INS(tw, (), (to, rA, rB), (), (opcd == 31, xo1 == 4, !_31), (), SPECIAL, "",
   (),
   (GPR[rA], GPR[rB]))
INS(twi, (), (to, rA, simm), (), (opcd == 3), (), SPECIAL, "",
   (),
   (GPR[rA]))

// Processor Control
INS(mcrxr, (crfD), (), (), (opcd == 31, xo1 == 512, !_9_10, !_11_15, !_16_20, !_31), (), USER, "Move to Condition Register from XER",
   (CR[crfD][LT,GT,EQ,SO], XER[CRXR]),
   (XER[CRXR]))
INS(mfcr, (rD), (), (), (opcd == 31, xo1 == 19, !_11_15, !_16_20, !_31), (), USER, "Move from Condition Register", 
   (GPR[rD]),
   (CR))
INS(mfspr, (rD), (spr), (), (opcd == 31, xo1 == 339, !_31), (), USER, "Move from Special Purpose Register", 
   (GPR[rD]), 
   (SPR[spr]))
INS(mftb, (rD), (tbr), (), (opcd == 31, xo1 == 371, !_31), (), USER, "Move from Time Base Register", 
   (GPR[rD]), 
   (TBR[tbr]))
INS(mtcrf, (crm), (rS), (), (opcd == 31, xo1 == 144, !_11, !_20, !_31), (), USER, "Move to Condition Register Fields",
   (CR[FIELDMASK(crm)]),
   (GPR[rS]))
INS(mtspr, (spr), (rS), (), (opcd == 31, xo1 == 467, !_31), (), USER, "Move to Special Purpose Register", 
   (SPR[spr]), 
   (GPR[rS]))

// Processor Control - Super
INS(mfmsr, (rD), (), (), (opcd == 31, xo1 == 83, !_11_15, !_16_20, !_31), (), SUPER, "Move from Machine State Register", (), ())
INS(mtmsr, (), (rS), (), (opcd == 31, xo1 == 146, !_11_15, !_16_20, !_31), (), SUPER, "Move to Machine State Register", (), ())

// Cache Management
INS(dcbf, (), (rA, rB), (), (opcd == 31, xo1 == 86, !_6_10, !_31), (), SPECIAL, "", (), ())
INS(dcbi, (), (rA, rB), (), (opcd == 31, xo1 == 470, !_6_10, !_31), (), SPECIAL, "", (), ())
INS(dcbst, (), (rA, rB), (), (opcd == 31, xo1 == 54, !_6_10, !_31), (), SPECIAL, "", (), ())
INS(dcbt, (), (rA, rB), (), (opcd == 31, xo1 == 278, !_6_10, !_31), (), SPECIAL, "", (), ())
INS(dcbtst, (), (rA, rB), (), (opcd == 31, xo1 == 246, !_6_10, !_31), (), SPECIAL, "", (), ())
INS(dcbz, (), (rA, rB), (), (opcd == 31, xo1 == 1014, !_6_10, !_31), (), SPECIAL, "", (), ())
INS(icbi, (), (rA, rB), (), (opcd == 31, xo1 == 982, !_6_10, !_31), (), SPECIAL, "", (), ())
INS(dcbz_l, (), (rA, rB), (), (opcd == 4, xo1 == 1014, !_6_10, !_31), (), SPECIAL, "", (), ())

// Segment Register Manipulation
INS(mfsr, (rD), (sr), (), (opcd == 31, xo1 == 595, !_11, !_16_20, !_31), (), SUPER, "Move from Segment Register", (), ())
INS(mfsrin, (rD), (rB), (), (opcd == 31, xo1 == 659, !_11_15, !_31), (), SUPER, "Move from Segment Register Indirect", (), ())
INS(mtsr, (), (rD, sr), (), (opcd == 31, xo1 == 210, !_11, !_16_20, !_31), (), SUPER, "Move to Segment Register", (), ())
INS(mtsrin, (), (rD, rB), (), (opcd == 31, xo1 == 242, !_11_15, !_31), (), SUPER, "Move to Segment Register Indirect", (), ())

// Lookaside Buffer Management
INS(tlbie, (), (rB), (), (opcd == 31, xo1 == 306, !_6_10, !_11_15, !_31), (), SUPER, "", (), ())
INS(tlbsync, (), (), (), (opcd == 31, xo1 == 566, !_6_10, !_11_15, !_16_20, !_31), (), SUPER, "", (), ())

// External Control
INS(eciwx, (rD), (rA, rB), (), (opcd == 31, xo1 == 310, !_31), (), SPECIAL, "", (), ())
INS(ecowx, (rD), (rA, rB), (), (opcd == 31, xo1 == 438, !_31), (), SPECIAL, "", (), ())

// Paired-Single Load and Store
INS(psq_l, (frD), (rA, qd), (w, i), (opcd == 56), (), SPECIAL, "Paired Single Load", (), ())
INS(psq_lu, (frD), (rA, qd), (w, i), (opcd == 57), (rA == 0), SPECIAL, "Paired Single Load with Update", (), ())
INS(psq_lx, (frD), (rA, rB), (qw, qi), (opcd == 4, xo3 == 6, !_31), (), SPECIAL, "Paired Single Load Indexed", (), ())
INS(psq_lux, (frD), (rA, rB), (qw, qi), (opcd == 4, xo3 == 38, !_31), (rA == 0), SPECIAL, "Paired Single Load with Update Indexed", (), ())
INS(psq_st, (frD), (rA, qd), (w, i), (opcd == 60), (), SPECIAL, "Paired Single Store", (), ())
INS(psq_stu, (frD), (rA, qd), (w, i), (opcd == 61), (), SPECIAL, "Paired Single Store with Update", (), ())
INS(psq_stx, (frS), (rA, rB), (qw, qi), (opcd == 4, xo3 == 7, !_31), (), SPECIAL, "Paired Single Store Indexed", (), ())
INS(psq_stux, (frS), (rA, rB), (qw, qi), (opcd == 4, xo3 == 39, !_31), (), SPECIAL, "Paired Single Store with Update Indexed", (), ())

// Paired-Single Floating Point Arithmetic
INS(ps_add, (frD), (frA, frB), (Rc), (opcd == 4, xo4 == 21, !_21_25), (), USER, "Paired Single Add",
   (PSR[rD], FPSCR[FPRF, FR, FI, FX, OX, UX, XX, VXSNAN, VXISI], Rc.on(CR[1][FX, FEX, VX, OX])),
   (PSR[rA], PSR[rB], FPSCR[VE]))
INS(ps_div, (frD), (frA, frB), (Rc), (opcd == 4, xo4 == 18, !_21_25), (), USER, "Paired Single Divide", (), ())
INS(ps_mul, (frD), (frA, frC), (Rc), (opcd == 4, xo4 == 25, !_16_20), (), USER, "Paired Single Multiply", (), ())
INS(ps_sub, (frD), (frA, frB), (Rc), (opcd == 4, xo4 == 20, !_21_25), (), USER, "Paired Single Subtract", (), ())
INS(ps_abs, (frD), (frB), (Rc), (opcd == 4, xo1 == 264, !_11_15), (), USER, "Paired Single Absolute", (), ())
INS(ps_nabs, (frD), (frB), (Rc), (opcd == 4, xo1 == 136, !_11_15), (), USER, "Paired Single Negate Absolute", (), ())
INS(ps_neg, (frD), (frB), (Rc), (opcd == 4, xo1 == 40, !_11_15), (), USER, "Paired Single Negate", (), ())
INS(ps_sel, (frD), (frA, frB, frC), (Rc), (opcd == 4, xo4 == 23), (), USER, "Paired Single Select", (), ())
INS(ps_res, (frD), (frB), (Rc), (opcd == 4, xo4 == 24, !_11_15, !_21_25), (), USER, "Paired Single Reciprocal", (), ())
INS(ps_rsqrte, (frD), (frB), (Rc), (opcd == 4, xo4 == 26, !_11_15, !_21_25), (), USER, "Paired Single Reciprocal Square Root Estimate", (), ())
INS(ps_msub, (frD), (frA, frB, frC), (Rc), (opcd == 4, xo4 == 28), (), USER, "Paired Single Multiply and Subtract", (), ())
INS(ps_madd, (frD), (frA, frB, frC), (Rc), (opcd == 4, xo4 == 29), (), USER, "Paired Single Multiply and Add", (), ())
INS(ps_nmsub, (frD), (frA, frB, frC), (Rc), (opcd == 4, xo4 == 30), (), USER, "Paired Single Negate Multiply and Subtract", (), ())
INS(ps_nmadd, (frD), (frA, frB, frC), (Rc), (opcd == 4, xo4 == 31), (), USER, "Paired Single Negate Multiply and Add", (), ())
INS(ps_mr, (frD), (frB), (Rc), (opcd == 4, xo1 == 72, !_11_15), (), USER, "Paired Single Move Register", (), ())
INS(ps_sum0, (frD), (frA, frB, frC), (Rc), (opcd == 4, xo4 == 10), (), USER, "Paired Single Sum High", (), ())
INS(ps_sum1, (frD), (frA, frB, frC), (Rc), (opcd == 4, xo4 == 11), (), USER, "Paired Single Sum Low", (), ())
INS(ps_muls0, (frD), (frA, frC), (Rc), (opcd == 4, xo4 == 12, !_16_20), (), USER, "Paired Single Multiply Scalar High", (), ())
INS(ps_muls1, (frD), (frA, frC), (Rc), (opcd == 4, xo4 == 13, !_16_20), (), USER, "Paired Single Multiply Scalar Low", (), ())
INS(ps_madds0, (frD), (frA, frB, frC), (Rc), (opcd == 4, xo4 == 14), (), USER, "Paired Single Multiply and Add Scalar High", (), ())
INS(ps_madds1, (frD), (frA, frB, frC), (Rc), (opcd == 4, xo4 == 15), (), USER, "Paired Single Multiply and Add Scalar Low", (), ())
INS(ps_cmpu0, (crfD), (frA, frB), (), (opcd == 4, xo1 == 0, !_9_10, !_31), (), USER, "Paired Single Compare Unordered High", (), ())
INS(ps_cmpo0, (crfD), (frA, frB), (), (opcd == 4, xo1 == 32, !_9_10, !_31), (), USER, "Paired Single Compare Ordered High", (), ())
INS(ps_cmpu1, (crfD), (frA, frB), (), (opcd == 4, xo1 == 64, !_9_10, !_31), (), USER, "Paired Single Compare Unordered Low", (), ())
INS(ps_cmpo1, (crfD), (frA, frB), (), (opcd == 4, xo1 == 96, !_9_10, !_31), (), USER, "Paired Single Compare Ordered Low", (), ())
INS(ps_merge00, (frD), (frA, frB), (Rc), (opcd == 4, xo1 == 528), (), USER, "Paired Single Merge High", (), ())
INS(ps_merge01, (frD), (frA, frB), (Rc), (opcd == 4, xo1 == 560), (), USER, "Paired Single Merge Direct", (), ())
INS(ps_merge10, (frD), (frA, frB), (Rc), (opcd == 4, xo1 == 592), (), USER, "Paired Single Merge Swapped", (), ())
INS(ps_merge11, (frD), (frA, frB), (Rc), (opcd == 4, xo1 == 624), (), USER, "Paired Single Merge Low", (), ())
