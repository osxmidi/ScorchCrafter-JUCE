#include "tonestack_a.h"

ToneStack_A::ToneStack_A(fsx nRate) :
	sRate(fhx (nRate)),
	setShaped(false),
	fpPrec(int (1))
{
	if(fhx (sRate) < fhx (44100.0)) sRate = fhx (44100.0);

	s_bValue[0] = fsx (0.50);
	s_bValue[1] = fsx (0.50);
	s_bValue[2] = fsx (0.50);
	s_bValue[3] = fsx (0.00);
	s_bValue[4] = fsx (0.00);
	s_bValue[5] = fsx (0.00);

	for(int x = 0; x < 6; x++)
	{
		l_bValue[x] = flx (s_bValue[x]);
		h_bValue[x] = fhx (s_bValue[x]);
	}

	h_EqBand_Freq[0][0] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 30.0 / fhx (sRate)));
	h_EqBand_Freq[1][0] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 300.0 / fhx (sRate)));
	h_EqBand_Freq[2][0] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 120.0 / fhx (sRate)));
	h_EqBand_Freq[3][0] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 1000.0 / fhx (sRate)));
	h_EqBand_Freq[4][0] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 900.0 / fhx (sRate)));
	h_EqBand_Freq[5][0] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 3000.0 / fhx (sRate)));

	h_EqBand_Freq[0][1] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 120.0 / fhx (sRate)));
	h_EqBand_Freq[1][1] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 500.0 / fhx (sRate)));
	h_EqBand_Freq[2][1] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 900.0 / fhx (sRate)));
	h_EqBand_Freq[3][1] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 2000.0 / fhx (sRate)));
	h_EqBand_Freq[4][1] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 20000.0 / fhx (sRate)));
	h_EqBand_Freq[5][1] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 20000.0 / fhx (sRate)));

	h_outputF_a_frq = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 7200.0 / fhx (sRate)));
	h_outputF_b_frq = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 15000.0 / fhx (sRate)));
	s_outputF_a_frq = fsx (h_outputF_a_frq);
	s_outputF_b_frq = fsx (h_outputF_b_frq);
	l_outputF_a_frq = flx (h_outputF_a_frq);
	l_outputF_b_frq = flx (h_outputF_b_frq);

	l_outputF_a_pos = flx (0.0f);
	l_outputF_b_pos = flx (0.0f);
	s_outputF_a_pos = fsx (0.0);
	s_outputF_b_pos = fsx (0.0);
	h_outputF_a_pos = fhx (0.0);
	h_outputF_b_pos = fhx (0.0);

	for (int x = 0; x < 6; x++)
	{
		l_EqBand_Pos[x][0] = flx (0.0);
		l_EqBand_Pos[x][1] = flx (0.0);
		s_EqBand_Pos[x][0] = fsx (0.0);
		s_EqBand_Pos[x][1] = fsx (0.0);
		h_EqBand_Pos[x][0] = fhx (0.0);
		h_EqBand_Pos[x][1] = fhx (0.0);

		l_EqBand_Freq[x][0] = flx (h_EqBand_Freq[x][0]);
		l_EqBand_Freq[x][1] = flx (h_EqBand_Freq[x][1]);
		s_EqBand_Freq[x][0] = fsx (h_EqBand_Freq[x][0]);
		s_EqBand_Freq[x][1] = fsx (h_EqBand_Freq[x][1]);
	}

	DeFizzFL = new BTWorthLP_LD(fsx (5000.0), fsx (sRate));
	DeFizzFS = new BTWorthLP_SD(fsx (5000.0), fsx (sRate));
	DeFizzFH = new BTWorthLP_HD(fsx (7200.0), fsx (sRate));
}
ToneStack_A::~ToneStack_A()
{
	delete DeFizzFL;
	delete DeFizzFS;
	delete DeFizzFH;
}
void ToneStack_A::setNewSampleRate(const fsx nRate)
{
	sRate = fhx (nRate);

	if(fhx (sRate) < fhx (44100.0)) sRate = fhx (44100.0);

	h_EqBand_Freq[0][0] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 30.0 / fhx (sRate)));
	h_EqBand_Freq[1][0] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 300.0 / fhx (sRate)));
	h_EqBand_Freq[2][0] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 120.0 / fhx (sRate)));
	h_EqBand_Freq[3][0] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 1000.0 / fhx (sRate)));
	h_EqBand_Freq[4][0] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 900.0 / fhx (sRate)));
	h_EqBand_Freq[5][0] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 3000.0 / fhx (sRate)));

	h_EqBand_Freq[0][1] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 120.0 / fhx (sRate)));
	h_EqBand_Freq[1][1] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 500.0 / fhx (sRate)));
	h_EqBand_Freq[2][1] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 900.0 / fhx (sRate)));
	h_EqBand_Freq[3][1] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 2000.0 / fhx (sRate)));
	h_EqBand_Freq[4][1] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 20000.0 / fhx (sRate)));
	h_EqBand_Freq[5][1] = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 20000.0 / fhx (sRate)));

	for (int x = 0; x < 6; x++)
	{
		l_EqBand_Freq[x][0] = flx (h_EqBand_Freq[x][0]);
		l_EqBand_Freq[x][1] = flx (h_EqBand_Freq[x][1]);
		s_EqBand_Freq[x][0] = fsx (h_EqBand_Freq[x][0]);
		s_EqBand_Freq[x][1] = fsx (h_EqBand_Freq[x][1]);
	}

	h_outputF_a_frq = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 7200.0 / fhx (sRate)));
	h_outputF_b_frq = fhx ((fhx) 1.0 - (fhx) expl((fhx) -2.0 * (fhx) M_PI * (fhx) 15000.0 / fhx (sRate)));
	s_outputF_a_frq = fsx (h_outputF_a_frq);
	s_outputF_b_frq = fsx (h_outputF_b_frq);
	l_outputF_a_frq = flx (h_outputF_a_frq);
	l_outputF_b_frq = flx (h_outputF_b_frq);

	DeFizzFL->setSampleRate(fsx (sRate));
	DeFizzFS->setSampleRate(fsx (sRate));
	DeFizzFH->setSampleRate(fsx (sRate));

	ClearBuffs();
}
void ToneStack_A::setBandValue(const int bNum, const fsx value)
{
	if(bNum < 0) return;
	if(bNum > 5) return;

	l_bValue[bNum] = flx (value);
	s_bValue[bNum] = fsx (value);
	h_bValue[bNum] = fhx (value);
}
void ToneStack_A::setEvenShaped(const bool isShaped)
{
	if(isShaped != setShaped) ClearBuffs();
	setShaped = isShaped;
}
fsx ToneStack_A::GetProcessedOutput()
{
	if(fpPrec == 0) return fsx (l_spl);
	if(fpPrec == 1) return fsx (s_spl);
	if(fpPrec == 2) return fsx (h_spl);

	return fsx (0.0);
}
void ToneStack_A::ClearBuffs()
{
	for (int x = 0; x < 6; x++)
	{
		l_EqBand_Pos[x][0] = flx (0.0);
		l_EqBand_Pos[x][1] = flx (0.0);
		s_EqBand_Pos[x][0] = fsx (0.0);
		s_EqBand_Pos[x][1] = fsx (0.0);
		h_EqBand_Pos[x][0] = fhx (0.0);
		h_EqBand_Pos[x][1] = fhx (0.0);
	}

	l_outputF_a_pos = flx (0.0f);
	l_outputF_b_pos = flx (0.0f);
	s_outputF_a_pos = fsx (0.0);
	s_outputF_b_pos = fsx (0.0);
	h_outputF_a_pos = fhx (0.0);
	h_outputF_b_pos = fhx (0.0);
}
void ToneStack_A::SetInput(const fsx inp)
{
	h_spl = fhx (inp);
	s_spl = fsx (inp);
	l_spl = flx (inp);
}
void ToneStack_A::SetPrecision(int nLevel)
{
	if(nLevel < 0) nLevel = 0;
	if(nLevel > 2) nLevel = 2;

	fpPrec = nLevel;
}
void ToneStack_A::RunEQ()
{
	if(fpPrec == 0)
	{
		l_subSpl[0] = flx (l_spl);
		l_subSpl[1] = flx (l_spl);
		l_subSpl[2] = flx (l_spl);
		l_subSpl[3] = flx (l_spl);
		l_subSpl[4] = flx (l_spl);
		l_subSpl[5] = flx (l_spl);

		for(int z = 0; z < 6; z++)
		{
			l_subSpl[z] -= flx (l_EqBand_Pos[z][0] += flx (flx ((flx) l_subSpl[z] - (flx) l_EqBand_Pos[z][0]) * flx (l_EqBand_Freq[z][0])));
			l_subSpl[z] = flx (l_EqBand_Pos[z][1] += flx (flx ((flx) l_subSpl[z] - (flx) l_EqBand_Pos[z][1]) * flx (l_EqBand_Freq[z][1])));
		}
		l_subSpl[0] *= flx (flx (flx (flx (l_bValue[0]) * flx (l_bValue[0])) * flx (2.0f)) - flx (0.50f));
		l_subSpl[2] *= flx (flx (flx (flx (l_bValue[1]) * flx (l_bValue[1])) * flx (2.0f)) - flx (0.50f));
		l_subSpl[4] *= flx (flx (flx (flx (l_bValue[2]) * flx (l_bValue[2])) * flx (2.0f)) - flx (0.50f));
		l_subSpl[1] *= flx (flx (flx (l_bValue[3]) * flx (l_bValue[3])) * flx (2.0f));
		l_subSpl[3] *= flx (flx (flx (l_bValue[4]) * flx (l_bValue[4])) * flx (2.0f));
		l_subSpl[5] *= flx (flx (flx (l_bValue[5]) * flx (l_bValue[5])) * flx (2.0f));

		l_spl += flx (flx (l_subSpl[0]) + flx (l_subSpl[1]) + flx (l_subSpl[2]) + flx (l_subSpl[3]) + flx (l_subSpl[4]) + flx (l_subSpl[5]));

		l_outputF_spl[0] = flx (l_spl);
		l_outputF_spl[1] = flx (l_spl);
		l_outputF_spl[0] = flx (l_outputF_a_pos += flx (flx ((flx) l_outputF_spl[0] - (flx) l_outputF_a_pos) * flx (l_outputF_a_frq)));
		l_outputF_spl[1] -= flx (l_outputF_b_pos += flx (flx ((flx) l_outputF_spl[1] - (flx) l_outputF_b_pos) * flx (l_outputF_b_frq)));

		l_spl = (flx) DeFizzFL->runFilterL(flx (l_spl));

		l_outputF_spl[0] *= flx (flx (l_bValue[0]) + flx (0.5f));
		l_outputF_spl[1] *= flx (flx (l_bValue[4]) + flx (0.5f));

		l_spl += flx (flx (l_outputF_spl[0]) + flx (l_outputF_spl[1]));
	}
	if(fpPrec == 1)
	{
		s_subSpl[0] = fsx (s_spl);
		s_subSpl[1] = fsx (s_spl);
		s_subSpl[2] = fsx (s_spl);
		s_subSpl[3] = fsx (s_spl);
		s_subSpl[4] = fsx (s_spl);
		s_subSpl[5] = fsx (s_spl);

		for(int z = 0; z < 6; z++)
		{
			s_subSpl[z] -= fsx (s_EqBand_Pos[z][0] += fsx (fsx ((fsx) s_subSpl[z] - (fsx) s_EqBand_Pos[z][0]) * fsx (s_EqBand_Freq[z][0])));
			s_subSpl[z] = fsx (s_EqBand_Pos[z][1] += fsx (fsx ((fsx) s_subSpl[z] - (fsx) s_EqBand_Pos[z][1]) * fsx (s_EqBand_Freq[z][1])));
		}
		s_subSpl[0] *= fsx (fsx (fsx (fsx (s_bValue[0]) * fsx (s_bValue[0])) * fsx (2.0)) - fsx (0.50));
		s_subSpl[2] *= fsx (fsx (fsx (fsx (s_bValue[1]) * fsx (s_bValue[1])) * fsx (2.0)) - fsx (0.50));
		s_subSpl[4] *= fsx (fsx (fsx (fsx (s_bValue[2]) * fsx (s_bValue[2])) * fsx (2.0)) - fsx (0.50));
		s_subSpl[1] *= fsx (fsx (fsx (s_bValue[3]) * fsx (s_bValue[3])) * fsx (2.0));
		s_subSpl[3] *= fsx (fsx (fsx (s_bValue[4]) * fsx (s_bValue[4])) * fsx (2.0));
		s_subSpl[5] *= fsx (fsx (fsx (s_bValue[5]) * fsx (s_bValue[5])) * fsx (2.0));

		s_spl += fsx (fsx (s_subSpl[0]) + fsx (s_subSpl[1]) + fsx (s_subSpl[2]) + fsx (s_subSpl[3]) + fsx (s_subSpl[4]) + fsx (s_subSpl[5]));

		s_outputF_spl[0] = fsx (s_spl);
		s_outputF_spl[1] = fsx (s_spl);
		s_outputF_spl[0] = fsx (s_outputF_a_pos += fsx (fsx ((fsx) s_outputF_spl[0] - (fsx) s_outputF_a_pos) * fsx (s_outputF_a_frq)));
		s_outputF_spl[1] -= fsx (s_outputF_b_pos += fsx (fsx ((fsx) s_outputF_spl[1] - (fsx) s_outputF_b_pos) * fsx (s_outputF_b_frq)));

		s_spl = (fsx) DeFizzFS->runFilterS(fsx (s_spl));

		s_outputF_spl[0] *= fsx (fsx (s_bValue[0]) + fsx (0.5));
		s_outputF_spl[1] *= fsx (fsx (s_bValue[4]) + fsx (0.5));

		s_spl += fsx (fsx (s_outputF_spl[0]) + fsx (s_outputF_spl[1]));
	}
	if(fpPrec == 2)
	{
//		h_spl = (fhx) DeFizzFH->runFilterH(fhx (h_spl));
		h_spl *= fhx (0.5000);

		h_subSpl[0] = fhx (h_spl);
		h_subSpl[1] = fhx (h_spl);
		h_subSpl[2] = fhx (h_spl);
		h_subSpl[3] = fhx (h_spl);
		h_subSpl[4] = fhx (h_spl);
		h_subSpl[5] = fhx (h_spl);

		for(int z = 0; z < 6; z++)
		{
			h_subSpl[z] -= fhx (h_EqBand_Pos[z][0] += fhx (fhx ((fhx) h_subSpl[z] - (fhx) h_EqBand_Pos[z][0]) * fhx (h_EqBand_Freq[z][0])));
			h_subSpl[z] = fhx (h_EqBand_Pos[z][1] += fhx (fhx ((fhx) h_subSpl[z] - (fhx) h_EqBand_Pos[z][1]) * fhx (h_EqBand_Freq[z][1])));
		}
		h_subSpl[0] *= fhx (fhx (h_bValue[0]) * fhx (8.0));
		h_subSpl[2] *= fhx (fhx (fhx (h_bValue[1]) * fhx (4.0)) - fhx (2.0));
		h_subSpl[4] *= fhx (fhx (fhx (h_bValue[2]) * fhx (8.0)) - fhx (2.0));
		h_subSpl[1] *= fhx (fhx (fhx (h_bValue[3]) * fhx (4.0)) );
		h_subSpl[3] *= fhx (fhx (fhx (h_bValue[4]) * fhx (4.0)) );
		h_subSpl[5] *= fhx (fhx (fhx (h_bValue[5]) * fhx (24.0)) - fhx (12.0));

		h_spl += fhx (fhx (h_subSpl[0]) + fhx (h_subSpl[1]) + fhx (h_subSpl[2]) + fhx (h_subSpl[3]) + fhx (h_subSpl[4]) + fhx (h_subSpl[5]));
		h_spl *= fhx (0.500);
/*
		h_outputF_spl[0] = fhx (h_spl);
		h_outputF_spl[1] = fhx (h_spl);
		h_outputF_spl[0] = fhx (h_outputF_a_pos += fhx (fhx ((fhx) h_outputF_spl[0] - (fhx) h_outputF_a_pos) * fhx (h_outputF_a_frq)));
		h_outputF_spl[1] -= fhx (h_outputF_b_pos += fhx (fhx ((fhx) h_outputF_spl[1] - (fhx) h_outputF_b_pos) * fhx (h_outputF_b_frq)));
*/
//		h_outputF_spl[0] *= fhx (fhx (h_bValue[0]) * fhx (4.0));
		// h_outputF_spl[1] *= fhx (fhx (h_bValue[5]) * fhx (0.250));

//		h_spl = fhx (fhx (h_outputF_spl[0]) + fhx (h_outputF_spl[1]));
		// h_spl *= fhx (2.0000);
	}
}
