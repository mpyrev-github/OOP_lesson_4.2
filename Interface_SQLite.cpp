//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Interface_SQLite.h"
#include "Alert.h"
#include <iostream>
#include <vector>
#include <cstring>
#include "sqlite3.h"
#include "windows.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VirtualTrees"
#pragma resource "*.dfm"
TsqliteForm *sqliteForm;
//---------------------------------------------------------------------------
__fastcall TsqliteForm::TsqliteForm(TComponent* Owner)
	: TForm(Owner)
{
	VST_SQLite->NodeDataSize = sizeof(VSTStructVariable);  // ������ ������ VST �������� ���������
}
//---------------------------------------------------------------------------
void __fastcall TsqliteForm::VST_SQLiteGetText(TBaseVirtualTree *Sender, PVirtualNode Node,
		  TColumnIndex Column, TVSTTextType TextType, UnicodeString &CellText)
		// ���������� ������� OnGetText ��� ����������� ����������� �������
{
	if (!Node) return;
	VSTStructVariable *nodeData = (VSTStructVariable *)Sender -> GetNodeData(Node);
	switch (Column){
		case 0: CellText = nodeData -> id;break;
		case 1: CellText = nodeData -> url;break;
		case 2: CellText = nodeData -> data; break;
	}
}
//---------------------------------------------------------------------------

void __fastcall TsqliteForm::pullButtonClick(TObject *Sender)
{
	VST_SQLite -> Clear();      // ������� ������
	sqlite3* db = NULL;
	sqlite3_open("places.sqlite", &db);     // ��������� ������������ ��

	if (db == NULL){     // ��������� ������� ���������� ��
		statusLabel -> Caption = "������ ��������/�������� ��.";
		return;
	}

	sqlite3_stmt *pullStatement;
	int result = sqlite3_prepare_v2(db,"select * from moz_places",
									-1, &pullStatement, NULL);      // ������� ������ �� ����� �������

	if (result != SQLITE_OK){        // ��������� ������� ������ SQL-�������
		const char *errmsg = sqlite3_errmsg(db);
		char errText[] =  "������ SQL-�������: ";
		statusLabel -> Caption = strcat(errText,errmsg);
	}
	else {
		statusLabel -> Caption = "���������� ��������";

		while (sqlite3_step(pullStatement) != SQLITE_DONE) {        // ���������� ������ ���� ������
			PVirtualNode entryNode = VST_SQLite -> AddChild(VST_SQLite -> RootNode);        // ��������� ��������� ������
			VSTStructVariable *nodeData = (VSTStructVariable*)VST_SQLite -> GetNodeData(entryNode);     // ��������� ������ ����
				nodeData -> id = sqlite3_column_int(pullStatement, 0);
				nodeData -> url  = (wchar_t*)sqlite3_column_text16(pullStatement, 1);
				nodeData -> data  = (wchar_t*)sqlite3_column_text16(pullStatement, 2);
		}
	}

	sqlite3_finalize(pullStatement);		// ������� Statement
	sqlite3_close(db);      // ��������� ��
}
//---------------------------------------------------------------------------
void __fastcall TsqliteForm::VST_SQLiteFreeNode(TBaseVirtualTree *Sender, PVirtualNode Node)
		// ���������� ������� OnFreeNode ��� ������� ����
{
	VSTStructVariable *nodeData = (VSTStructVariable *)Sender -> GetNodeData(Node);
	nodeData -> ~VSTStruct();
}
//---------------------------------------------------------------------------

void __fastcall TsqliteForm::delRowButtonClick(TObject *Sender)
{
	if (VST_SQLite -> FocusedNode == NULL) {        // ��������� ������� ���������� ��������� ������
		alertForm -> Visible=true;
		return;
	}

	PVirtualNode selectedNode = VST_SQLite -> FocusedNode;      // ������ ����� ���� �������
	int selectedNodeIndex = NULL;
	selectedNodeIndex = selectedNode -> Index;      // �������� ��� ����� ��� ������������ ���������
	int SelectedRowCount = VST_SQLite->SelectedCount;       // ���������� ���������� �������
	std::vector<PVirtualNode> selectedNodes;
	std::vector<VSTStructVariable *> nodeData;
	std::vector<int> idv;

	for (int i = 0; i < SelectedRowCount; i++) {
		  if (i==0) selectedNodes.push_back(VST_SQLite -> GetFirstSelected());
		  else selectedNodes.push_back(VST_SQLite -> GetNextSelected(selectedNodes[i-1]));
		  nodeData.push_back((VSTStructVariable *)VST_SQLite -> GetNodeData(selectedNodes[i]));		// ������� ������ ��� ��������������
		  idv.push_back(nodeData[i] -> id);
	}

	if (SelectedRowCount>0)
	{
		// ��������� ������� �������� �������/���������� ���� �� ������
		if ((VST_SQLite -> GetNext(selectedNodes[SelectedRowCount-1])==NULL) && (VST_SQLite -> GetLast(selectedNodes[SelectedRowCount-1])==NULL)) {
		   VST_SQLite -> RemoveFromSelection;
		}
		else {
			if (VST_SQLite -> GetNext(selectedNodes[SelectedRowCount-1])==NULL) {
				PVirtualNode selectedNext = VST_SQLite -> GetLast(selectedNodes[SelectedRowCount-1]);
				VST_SQLite -> AddToSelection(selectedNext);     // ������������ ��������� �� ���������� ����
			}
			else{
				PVirtualNode selectedNext = VST_SQLite -> GetNext(selectedNodes[SelectedRowCount-1]);
				VST_SQLite -> AddToSelection(selectedNext);     // ������������ ��������� �� ��������� ����
            }
		}

		sqlite3* db;
		sqlite3_open("places.sqlite",&db);      // ��������� ������������ ��

			for (int i = 0; i < SelectedRowCount; i++) {
				VST_SQLite -> DeleteNode(selectedNodes [i]);        // ������ ����
				char buf[10] = "";      // ������� ����� ��� ������� id �� 10 ������� �����
				itoa(idv[i], buf, 10);		// ���������� � ����� �������� �� ������� id � ���������� ����
				sqlite3_stmt *delRowStatement;
				char sql[] = "delete from moz_places where id=";     // ������� ������ �� �������� �� ��
				strcat(sql, buf);       // ��������� �������� id � SQL-������� �� ������
				int result = sqlite3_prepare_v2(db, sql, -1, &delRowStatement, NULL);       // ������� �� ��
				result = sqlite3_step(delRowStatement);

				if(result != SQLITE_OK){		// ��������� ������� ������ SQL-�������
				   const char * errmsg = sqlite3_errmsg(db);
				   char errText[] =  "������ SQL-�������: ";
				   statusLabel -> Caption = strcat(errText,errmsg);
				}

				sqlite3_finalize(delRowStatement);      // ������� Statement
			}

		sqlite3_close(db);      // ��������� ��
		statusLabel -> Caption = "������ �������!";
	}
}
//---------------------------------------------------------------------------

void __fastcall TsqliteForm::clearTableButtonClick(TObject *Sender)
{
	VST_SQLite -> BeginUpdate();        // ����������� Begin/EndUpdate ��� ��������� �������� ���������� �����
	sqlite3* db;
	sqlite3_open("places.sqlite",&db);      // ��������� ������������ ��
	char *errmsg = NULL;
	int result = sqlite3_exec(db, "delete from moz_places",
							NULL, NULL, &errmsg);		// �������� ������ �� ������� � ��

	if(result != SQLITE_OK){        // ��������� ������� ������ SQL-�������
		char errText[] = "������ SQL-�������: ";
		statusLabel -> Caption = strcat(errText,errmsg);
		return;
	}

	VST_SQLite -> Clear();      // ������� ������
	VST_SQLite -> EndUpdate();
	statusLabel -> Caption = "������� �������!";
	sqlite3_close(db);      // ��������� ��
}
//---------------------------------------------------------------------------

void __fastcall TsqliteForm::closeButtonClick(TObject *Sender)
{
	Close();        // ��������� ���������
}
//---------------------------------------------------------------------------

