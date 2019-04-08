//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Alert.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TalertForm *alertForm;
//---------------------------------------------------------------------------
__fastcall TalertForm::TalertForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TalertForm::okButtonClick(TObject *Sender)
{
    alertForm -> Visible = false;
}
//---------------------------------------------------------------------------
