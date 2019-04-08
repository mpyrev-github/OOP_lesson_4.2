//---------------------------------------------------------------------------

#ifndef AlertH
#define AlertH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TalertForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *alertLabel;
	TButton *okButton;
	void __fastcall okButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TalertForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TalertForm *alertForm;
//---------------------------------------------------------------------------
#endif
