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
	VST_SQLite->NodeDataSize = sizeof(VSTStructVariable);  // Задаем размер VST согласно структуре
}
//---------------------------------------------------------------------------
void __fastcall TsqliteForm::VST_SQLiteGetText(TBaseVirtualTree *Sender, PVirtualNode Node,
		  TColumnIndex Column, TVSTTextType TextType, UnicodeString &CellText)
		// Обработчик события OnGetText для отображения содержимого таблицы
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
	VST_SQLite -> Clear();      // Очищаем дерево
	sqlite3* db = NULL;
	sqlite3_open("places.sqlite", &db);     // Открываем интересующую БД

	if (db == NULL){     // Обработка события отсутствия БД
		statusLabel -> Caption = "Ошибка открытия/создания БД.";
		return;
	}

	sqlite3_stmt *pullStatement;
	int result = sqlite3_prepare_v2(db,"select * from moz_places",
					-1, &pullStatement, NULL);      // Создаем запрос на вывод таблицы

	if (result != SQLITE_OK){        // Обработка события ошибки SQL-запроса
		const char *errmsg = sqlite3_errmsg(db);
		char errText[] =  "Ошибка SQL-запроса: ";
		statusLabel -> Caption = strcat(errText,errmsg);
	}
	else {
		statusLabel -> Caption = "Результаты получены";

		while (sqlite3_step(pullStatement) != SQLITE_DONE) {        // Перебираем записи базы данных
			PVirtualNode entryNode = VST_SQLite -> AddChild(VST_SQLite -> RootNode);        // Добавляем очередную запись
			VSTStructVariable *nodeData = (VSTStructVariable*)VST_SQLite -> GetNodeData(entryNode);     // Заполняем данные узла
				nodeData -> id = sqlite3_column_int(pullStatement, 0);
				nodeData -> url  = (wchar_t*)sqlite3_column_text16(pullStatement, 1);
				nodeData -> data  = (wchar_t*)sqlite3_column_text16(pullStatement, 2);
		}
	}

	sqlite3_finalize(pullStatement);		// Очищаем Statement
	sqlite3_close(db);      // Закрываем БД
}
//---------------------------------------------------------------------------
void __fastcall TsqliteForm::VST_SQLiteFreeNode(TBaseVirtualTree *Sender, PVirtualNode Node)
		// Обработчик события OnFreeNode для очистки узла
{
	VSTStructVariable *nodeData = (VSTStructVariable *)Sender -> GetNodeData(Node);
	nodeData -> ~VSTStruct();
}
//---------------------------------------------------------------------------

void __fastcall TsqliteForm::delRowButtonClick(TObject *Sender)
{
	if (VST_SQLite -> FocusedNode == NULL) {        // Обработка события отсутствия выбранной строки
		alertForm -> Visible=true;
		return;
	}

	PVirtualNode selectedNode = VST_SQLite -> FocusedNode;      // Узнаем какой узел выделен
	int selectedNodeIndex = NULL;
	selectedNodeIndex = selectedNode -> Index;      // Сохраним его номер для последующего выделения
	int SelectedRowCount = VST_SQLite->SelectedCount;       // Количество выделенных записей
	std::vector<PVirtualNode> selectedNodes;
	std::vector<VSTStructVariable *> nodeData;
	std::vector<int> idv;

	for (int i = 0; i < SelectedRowCount; i++) {
		  if (i==0) selectedNodes.push_back(VST_SQLite -> GetFirstSelected());
		  else selectedNodes.push_back(VST_SQLite -> GetNextSelected(selectedNodes[i-1]));
		  nodeData.push_back((VSTStructVariable *)VST_SQLite -> GetNodeData(selectedNodes[i]));		// Получим дерево для редактирования
		  idv.push_back(nodeData[i] -> id);
	}

	if (SelectedRowCount>0)
	{
		// Обработка события удаления первого/последнего узла из списка
		if ((VST_SQLite -> GetNext(selectedNodes[SelectedRowCount-1])==NULL) && (VST_SQLite -> GetLast(selectedNodes[SelectedRowCount-1])==NULL)) {
		   VST_SQLite -> RemoveFromSelection(selectedNode);
		}
		else {
			if (VST_SQLite -> GetNext(selectedNodes[SelectedRowCount-1])==NULL) {
				PVirtualNode selectedLast = VST_SQLite -> GetLast(selectedNodes[SelectedRowCount-1]);
				VST_SQLite -> AddToSelection(selectedLast);     // Переставляем выделение на предыдущий узел
			}
			else{
				PVirtualNode selectedNext = VST_SQLite -> GetNext(selectedNodes[SelectedRowCount-1]);
				VST_SQLite -> AddToSelection(selectedNext);     // Переставляем выделение на слудующий узел
            }
		}

		sqlite3* db;
		sqlite3_open("places.sqlite",&db);      // Открываем интересующую БД

			for (int i = 0; i < SelectedRowCount; i++) {
				VST_SQLite -> DeleteNode(selectedNodes [i]);        // Удалим узел
				char buf[10] = "";      // Создаем буфер для вектора id до 10 значных чисел
				itoa(idv[i], buf, 10);		// Записываем в буфер значение из вектора id в десятичном виде
				sqlite3_stmt *delRowStatement;
				char sql[] = "delete from moz_places where id=";     // Создаем запрос на удаление из БД
				strcat(sql, buf);       // Добавляем значение id к SQL-запросу из буфера
				int result = sqlite3_prepare_v2(db, sql, -1, &delRowStatement, NULL);       // Удаляем из БД
				result = sqlite3_step(delRowStatement);

				if(result != SQLITE_OK){		// Обработка события ошибки SQL-запроса
				   const char * errmsg = sqlite3_errmsg(db);
				   char errText[] =  "Ошибка SQL-запроса: ";
				   statusLabel -> Caption = strcat(errText,errmsg);
				}

				sqlite3_finalize(delRowStatement);      // Очищаем Statement
			}

		sqlite3_close(db);      // Закрываем БД
		statusLabel -> Caption = "Записи удалены!";
	}
}
//---------------------------------------------------------------------------

void __fastcall TsqliteForm::clearTableButtonClick(TObject *Sender)
{
	VST_SQLite -> BeginUpdate();        // Конструкция Begin/EndUpdate для изменения большого количества узлов
	sqlite3* db;
	sqlite3_open("places.sqlite",&db);      // Открываем интересующую БД
	char *errmsg = NULL;
	int result = sqlite3_exec(db, "delete from moz_places",
							NULL, NULL, &errmsg);		// Удаление данных из таблицы в БД

	if(result != SQLITE_OK){        // Обработка события ошибки SQL-запроса
		char errText[] = "Ошибка SQL-запроса: ";
		statusLabel -> Caption = strcat(errText,errmsg);
		return;
	}

	VST_SQLite -> Clear();      // Очищаем дерево
	VST_SQLite -> EndUpdate();
	statusLabel -> Caption = "Таблица очищена!";
	sqlite3_close(db);      // Закрываем БД
}
//---------------------------------------------------------------------------

void __fastcall TsqliteForm::closeButtonClick(TObject *Sender)
{
	Close();        // Закрываем программу
}
//---------------------------------------------------------------------------

