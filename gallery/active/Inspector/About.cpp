//$$---- activex aboutbox source ---- (stAboutBoxSource)
//---------------------------------------------------------------------------


#include <vcl.h>
#pragma hdrstop

#include "About.h"
//---------------------------------------------------------------------------


#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
void ShowInspectorAbout(void)
{
  TInspectorAbout* Form;
  Form = new TInspectorAbout(NULL);
  try
  {
    Form->ShowModal();
  }
  catch(...)
  {
    Form->Free();
    return;
  }
  Form->Free();
}
//---------------------------------------------------------------------------
__fastcall TInspectorAbout::TInspectorAbout(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
