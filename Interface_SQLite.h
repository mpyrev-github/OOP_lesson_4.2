//---------------------------------------------------------------------------

#ifndef Interface_SQLiteH
#define Interface_SQLiteH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "VirtualTrees.hpp"
//---------------------------------------------------------------------------
typedef struct VSTStruct {  // Создаём структуру для VST
	int id;                 // Описываем столбцы, указываем тип данных
	UnicodeString url;
	UnicodeString data;
}VSTStructVariable;         // Создаем переменную структуры VST
//---------------------------------------------------------------------------
class TsqliteForm : public TForm
{
__published:	// IDE-managed Components
	TVirtualStringTree *VST_SQLite;
	TButton *pullButton;
	TButton *delRowButton;
	TButton *clearTableButton;
	TLabel *statusLabel;
	TButton *closeButton;
	void __fastcall VST_SQLiteGetText(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
          TVSTTextType TextType, UnicodeString &CellText);
	void __fastcall pullButtonClick(TObject *Sender);
	void __fastcall VST_SQLiteFreeNode(TBaseVirtualTree *Sender, PVirtualNode Node);
	void __fastcall delRowButtonClick(TObject *Sender);
	void __fastcall clearTableButtonClick(TObject *Sender);
	void __fastcall closeButtonClick(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TsqliteForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TsqliteForm *sqliteForm;
//---------------------------------------------------------------------------
#endif
