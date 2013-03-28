//$$---- activex aboutbox header ---- (stAboutBoxHeader)
//---------------------------------------------------------------------------


#ifndef AboutH
#define AboutH
//---------------------------------------------------------------------------
#include <classes.hpp>
#include <controls.hpp>
#include <stdCtrls.hpp>
#include <forms.hpp>
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TInspectorAbout : public TForm
{
__published:	// IDE-managed Components
        TSpeedButton *CtlImage;
        TLabel *NameLbl;
        TLabel *DescLbl;
        TLabel *CopyrightLbl;
        TButton *OkBtn;
private:	// User declarations
public:		// User declarations
  __fastcall TInspectorAbout(TComponent* Owner);
};
//---------------------------------------------------------------------------
void ShowInspectorAbout(void);
//---------------------------------------------------------------------------
#endif
