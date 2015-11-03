#pragma once


#include "Using.h"
#include "InspectorData.h"

//using namespace System::Windows::Input;
//using namespace System::Windows;
using System::Windows::Controls::MenuItem;
using System::Runtime::InteropServices::Marshal;
static int cre = 0;
ref class GameScreenPanel: public System::Windows::Forms::Panel{
public:
	property System::Windows::Forms::CreateParams^ CreateParams
	{
		virtual System::Windows::Forms::CreateParams^ get() override
		{
			System::Windows::Forms::CreateParams ^cp = __super::CreateParams;// System::Windows::Forms::Panel::CreateParams;
			//cp->ExStyle |= WS_EX_TRANSPARENT;// 0x00000020;//�N���b�N����
			//cp->Style == NULL;
			//cp->Caption = "aiuto";
			//cp->ExStyle == NULL;
			return cp;
		}
	}
	virtual void OnPaintBackground(System::Windows::Forms::PaintEventArgs pevent) override
	{
	}
	virtual void OnPaint(System::Windows::Forms::PaintEventArgs pevent) override
	{
	}
public:
	void UpdateStyle(){
		SetStyle(System::Windows::Forms::ControlStyles::SupportsTransparentBackColor, true);
		UpdateStyles();
	}
};

ref class GameScreen{
public:

	//void RemoveFrame()
	//{
	//	return;
	//	IntPtr hwnd = m_wfh->Child->Handle;
	//	System::Windows::MessageBox::Show(hwnd.ToString());
	//	//GetWindowLong((HWND)hwnd.ToPointer(), GWL_STYLE) ^ WS_THICKFRAME
	//	SetWindowLong((HWND)hwnd.ToPointer(), GWL_STYLE, NULL);
	//	SetWindowLong((HWND)hwnd.ToPointer(), GWL_EXSTYLE, WS_EX_TRANSPARENT);

	//	SetWindowPos((HWND)hwnd.ToPointer(), NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
	//	hwnd = m_wfh->Child->Parent->Handle;
	//	System::Windows::MessageBox::Show(hwnd.ToString());
	//	//GetWindowLong((HWND)hwnd.ToPointer(), GWL_STYLE) ^ WS_THICKFRAME
	//	SetWindowLong((HWND)hwnd.ToPointer(), GWL_STYLE, NULL);
	//	SetWindowLong((HWND)hwnd.ToPointer(), GWL_EXSTYLE, WS_EX_TRANSPARENT);

	//	SetWindowPos((HWND)hwnd.ToPointer(), NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
	//	
	//}

	template<class T>
	T^ HandleDragEnter(System::Windows::Forms::DragEventArgs ^e)
	{
		System::Reflection::FieldInfo ^info;

		System::Object ^obj;

		info = e->Data->GetType()->GetField("innerData", System::Reflection::BindingFlags::NonPublic | System::Reflection::BindingFlags::Instance);

		obj = info->GetValue(e->Data);

		info = obj->GetType()->GetField("innerData", System::Reflection::BindingFlags::NonPublic | System::Reflection::BindingFlags::Instance);

		System::Windows::DataObject ^dataObj = (System::Windows::DataObject^)info->GetValue(obj);

		T ^item = (T^)dataObj->GetData(T::typeid);
		return item;

	}

	GameScreen(WindowsFormsHost ^wfh)
		:m_wfh(wfh)
	{
		auto panel = gcnew GameScreenPanel();
		m_wfh->Child = panel;
		auto par = panel->Parent;
		//auto par2 = par->Parent;
		//par->Visible = false;
		//par->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &GameScreen::mu);
		//panel->BackColor = System::Drawing::Color::Transparent;
		//panel->Image = nullptr;
		//panel->UpdateStyle();

		//panel->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &GameScreen::mu);
		//auto par3 = par2->Parent;
		//par->BackColor = System::Drawing::Color::AliceBlue;
		//par->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &GameScreen::mu);
		//panel->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &GameScreen::md);
		//panel->UpdateStyle();
		//panel->BackColor = System::Drawing::Color::Transparent;
		
		//wfh->AllowDrop = true;
		//wfh->DragEnter += gcnew DragEventHandler(this, &GameScreen::OnDragOver2);
		//wfh->DragOver += gcnew DragEventHandler(this, &GameScreen::OnDragOver2);
		//wfh->MouseDown += gcnew MouseButtonEventHandler(this, &GameScreen::md2);
		wfh->Child->AllowDrop = true;
		wfh->Child->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &GameScreen::OnDrop);
		wfh->Child->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &GameScreen::OnDragOver);
		wfh->Child->DragOver += gcnew System::Windows::Forms::DragEventHandler(this, &GameScreen::OnDragOver);

		//auto image = gcnew System::Windows::Controls::Image();
		//mD3DImageEx = gcnew System::Windows::Interop::D3DImageEx();
		//image->Source = mD3DImageEx;
		//Gamepanel->Children->Add(image);
	}

	//void ml(Object^ sender, MouseButtonEventArgs ^e){
	//	*m_ML = (bool)e->LeftButton;
	//}
	//void mr(Object^ sender, MouseButtonEventArgs ^e){
	//	*m_MR = (bool)e->RightButton;
	//}



	void md(Object^ sender, System::Windows::Forms::MouseEventArgs ^e){
		if (e->Button == System::Windows::Forms::MouseButtons::Left){
			*m_ML = true;
		}
		if (e->Button == System::Windows::Forms::MouseButtons::Right){
			*m_MR = true;
		}
	}
	void mu(Object^ sender, System::Windows::Forms::MouseEventArgs ^e){
		if (e->Button == System::Windows::Forms::MouseButtons::Left){
			*m_ML = false;
		}
		if (e->Button == System::Windows::Forms::MouseButtons::Right){
			*m_MR = false;
		}
	}
	void mm(Object^ sender, System::Windows::Forms::MouseEventArgs ^e){
		//auto p = e->P(m_wfh);
		*m_MX = (int)e->X;
		*m_MY = (int)e->Y;
	}
	//void keypressnull(Object^ sender, System::Windows::Forms::KeyEventArgs ^e){
	//	e->Handled = true;
	//}
	//void keynull(Object^ sender, System::Windows::Forms::KeyEventArgs ^e){
	//	e->Handled = true;
	//}
	void SetMouseEvents(bool* l, bool* r, int* x, int* y){
		m_ML = l;
		m_MR = r;
		m_MX = x;
		m_MY = y;

		//mD3DImageEx->SetBackBufferEx(System::Windows::Interop::D3DResourceTypeEx::ID3D11Texture2D, IntPtr(pRenderTarget));

		//m_wfh->Child->MouseMove	+= gcnew MouseButtonEventHandler(this, &GameScreen::ml);
		//m_wfh->Child->MouseLeftButtonUp += gcnew MouseButtonEventHandler(this, &GameScreen::ml);
		//m_wfh->Child->MouseRightButtonDown += gcnew MouseButtonEventHandler(this, &GameScreen::mr);
		//m_wfh->Child->MouseRightButtonUp += gcnew MouseButtonEventHandler(this, &GameScreen::mr);

		m_wfh->Child->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &GameScreen::md);
		m_wfh->Child->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &GameScreen::mu);
		m_wfh->Child->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &GameScreen::mm);

		//m_wfh->Child->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &GameScreen::keynull);
	}

	//OnSourceInitialized�C�x���g�ȍ~����Ȃ��Ǝ擾�ł��Ȃ�
	HWND GetHWND(){
		//HWND���擾
		//auto source = gcnew System::Windows::Interop::WindowInteropHelper(this);
		//HwndSource ^source = (HwndSource^)HwndSource::FromVisual(this);
		//RemoveFrame();
		System::IntPtr handle = m_wfh->Child->Handle;

		return reinterpret_cast<HWND>(handle.ToPointer());
	}
	//�Q�[����ʗp��HWND���擾���邽�߂�WindowsFormsHost

private:

	void OnDrop(Object ^s, System::Windows::Forms::DragEventArgs ^e)
	{
		TextBlock ^t = HandleDragEnter<TextBlock>(e);
		//auto str = reinterpret_cast<String^>(e->Data->GetData(System::Windows::Forms::DataFormats::FileDrop));
		//System::Windows::MessageBox::Show(str[0] + " is dropped.", "Dropped!");
		//TextBlock ^t = (TextBlock^)e->Data->GetData(TextBlock::typeid);
		pin_ptr<const wchar_t> wch = PtrToStringChars(t->Text);
		size_t convertedChars = 0;
		size_t  sizeInBytes = ((t->Text->Length + 1) * 2);
		char    *ch = (char *)malloc(sizeInBytes);
		wcstombs_s(&convertedChars,
			ch, sizeInBytes,
			wch, sizeInBytes);
		std::string* str = new std::string(ch);
		Data::MyPostMessage(MyWindowMessage::CreatePrefabToActor, (void*)str);

		free(ch);
	}

	void OnDragOver(Object ^sender, System::Windows::Forms::DragEventArgs ^e)
	{
		e->Effect = System::Windows::Forms::DragDropEffects::Copy;
		return;
	}

	WindowsFormsHost ^m_wfh;
	bool* m_ML;
	bool* m_MR;
	int* m_MX;
	int* m_MY;
};

// �r���[
ref class View : public Window {
public:

	//void RemoveFrame()
	//{
	//	auto i = gcnew System::Windows::Interop::WindowInteropHelper(this);
	//	IntPtr hwnd = i->Handle;
	//	System::Windows::MessageBox::Show(hwnd.ToString());
	//	//GetWindowLong((HWND)hwnd.ToPointer(), GWL_STYLE) ^ WS_THICKFRAME
	//	SetWindowLong((HWND)hwnd.ToPointer(), GWL_EXSTYLE, WS_EX_TRANSPARENT);
	//	SetWindowLong((HWND)hwnd.ToPointer(), GWL_STYLE, NULL);

	//	SetWindowPos((HWND)hwnd.ToPointer(), NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
	//}

	View()
		: Window()
		, m_GameScreen(nullptr)
		, mGameScreenHWND(NULL)
		, m_ComponentPanel(nullptr)
		, m_TreeViewItemRoot(nullptr)
	{
		//DataContext = gcnew ViewModel();
		Background = Brushes::Black;
		//w:1184, h : 762
		Width = 1605 + 16 + 200;
		Height = 800 + 38;

		FrameworkElement ^contents = LoadContentsFromResource(IDR_VIEW);
		Content = contents;
		
		auto wfh = (WindowsFormsHost ^)contents->FindName("GameScreenWFH");
		m_GameScreen = gcnew GameScreen(wfh);

		m_ComponentPanel = (StackPanel ^)contents->FindName("MainDock");
		auto TreeViewDec = (Border ^)contents->FindName("TreeView");
		CreateTreeView(TreeViewDec);
		auto AssetTreeViewDec = (Border ^)contents->FindName("AssetTreeView");
		CreateAssetTreeView(AssetTreeViewDec);

		Title = "TenshiEngine";
		SetSelectItemReturnPoint();

		//((TestContent::Contents^)tv->DataContext)->TextChange("dacsdv");

		//System::Windows::Controls::TreeView a; 
		//auto i = a.Items;
		//i->CurrentItem

		//FrameworkElement ^component = LoadContentsFromResource(IDR_COMPONENT);
		//mainDock->Children->Add(component);

		//CreateComponent();

		//return;
		//
		//
		//Border ^baseBorder = (Border ^)contents->FindName("BaseBorder");
		//baseBorder->SetBinding(Border::BorderThicknessProperty, "Value");
		//baseBorder->SetBinding(Border::CornerRadiusProperty, "Value");
		////baseBorder->CornerRadius = System::Windows::CornerRadius(10,20,30,40);
		////baseBorder->SetValue(Border::CornerRadiusProperty, System::Windows::CornerRadius(10, 20, 30, 40));
		//
		//Slider ^valueSlider = (Slider ^)contents->FindName("ValueSlider");
		//valueSlider->SetBinding(Slider::ValueProperty, "Value");
		//
		//TextBlock ^valueTextBlock = (TextBlock ^)contents->FindName("ValueTextBlock");
		//valueTextBlock->SetBinding(TextBlock::TextProperty, "Value");
		//
		//
		//return;
		//
		//// Button �p�� Style ��ǉ�
		//System::Windows::Style ^buttonStyle = gcnew System::Windows::Style(System::Windows::Controls::Button::typeid);
		//buttonStyle->Setters->Add(gcnew System::Windows::Setter(Button::ForegroundProperty, gcnew SolidColorBrush(Color::FromRgb(240, 240, 240))));
		//buttonStyle->Setters->Add(gcnew System::Windows::Setter(Button::BackgroundProperty, gcnew SolidColorBrush(Color::FromRgb(5, 147, 14 * 16 + 2))));
		//buttonStyle->Setters->Add(gcnew System::Windows::Setter(Button::HeightProperty, (double)28));
		//buttonStyle->Setters->Add(gcnew System::Windows::Setter(Button::MarginProperty, gcnew System::Windows::Thickness(1)));
		//Resources->Add(Button::typeid, buttonStyle);
		//
		//// ��ԊO���� DockPanel ��ǉ�
		//DockPanel ^dockPanel = gcnew DockPanel();
		//Content = dockPanel;
		//
		//// [OK][�L�����Z��] ��u�� StackPanel ���쐬
		//StackPanel ^stackPanel = gcnew StackPanel();
		//stackPanel->Orientation = System::Windows::Controls::Orientation::Horizontal;
		//stackPanel->FlowDirection = System::Windows::FlowDirection::RightToLeft;
		//// StackPanel �Ƀ{�^����ǉ�
		//auto cb = gcnew Button();
		//cb->Content = "�L�����Z��";
		//cb->Width = 90;
		//stackPanel->Children->Add(cb);
		//auto ok = gcnew Button();
		//ok->Content = "OK";
		//ok->Width = 90;
		//stackPanel->Children->Add(ok);
		//
		//// StackPanel ��ǉ�
		//dockPanel->Children->Add(stackPanel);
		//DockPanel::SetDock(stackPanel, System::Windows::Controls::Dock::Bottom);
		//
		//// XAML�ō�������̂�ǉ�
		//dockPanel->Children->Add(contents);
		//DockPanel::SetDock(contents, System::Windows::Controls::Dock::Top);


	}

	void UpdateView(){
		if (m_ComponentPanel == nullptr)return;
		for (int i = 0; i < m_ComponentPanel->Children->Count; i++){
			auto b = dynamic_cast<Border^>(m_ComponentPanel->Children[i]);
			if (b == nullptr)continue;
			auto d = dynamic_cast<Panel^>(b->Child);
			if (d == nullptr)continue;
			ForDockPanelChild(d);
		}
	}
	void ForDockPanelChild(Panel^ dock){
		for (int i = 0; i < dock->Children->Count; i++){

			Panel^ d = dynamic_cast<Panel^>(dock->Children[i]);
			if (d != nullptr){
				ForDockPanelChild(d);
				continue;
			}

			CheckObject(dock->Children[i]);
		}
	}
	void CheckObject(UIElement^ ui){
		FrameworkElement^ t = dynamic_cast<FrameworkElement^>(ui);
		if (t == nullptr)return;
		IViewModel^ f = dynamic_cast<IViewModel^>(t->DataContext);
		if (f == nullptr)return;
		f->UpdateView();
	}
	void ClearAllComponent(){
		if (m_ComponentPanel == nullptr)return;
		m_ComponentPanel->Children->Clear();
	}

	void CreateComponent(array<InspectorData^>^ data){
		if (m_ComponentPanel == nullptr)return;
		FrameworkElement ^com = LoadContentsFromResource(IDR_COMPONENT);
		m_ComponentPanel->Children->Add(com);
		//DockPanel::SetDock(com, System::Windows::Controls::Dock::Top);
		auto dock = (DockPanel^)com->FindName("MainDock");
		int num = data->GetLength(0);
		for (int i = 0; i < num; i++){
			data[i]->CreateInspector(dock);
		}
	}

	void AddItem(String ^Name, IntPtr ptr){
		if (m_TreeViewItemRoot == nullptr)return;
		auto item = gcnew TestContent::Person(Name, gcnew TestContent::MyList());
		item->DataPtr = ptr;
		m_TreeViewItemRoot->Add(item);
		Data::MyPostMessage(MyWindowMessage::StackIntPtr, (void*)item->DataPtr);
		Data::MyPostMessage(MyWindowMessage::ReturnTreeViewIntPtr, (void*)item->ThisIntPtr);
	}

	void SetParent(IntPtr parent, IntPtr child){
		auto p = (gcroot<TestContent::Person^>*)(void*)parent;
		auto c = (gcroot<TestContent::Person^>*)(void*)child;
		(*p)->Add(*c);
	}

	//�X�V�C�x���g
	void ActorIntPtr_TargetUpdated(Object ^sender, System::Windows::Data::DataTransferEventArgs ^e){
		auto text = ((TextBlock^)sender);
		if (text->Text == ""){
			Data::MyPostMessage(MyWindowMessage::SelectActor, NULL);
			return;
		}
		//((IntViewModel^)text->DataContext)->Value = text->Text;
		//auto item = dynamic_cast<TestContent::Person^>(m_TreeView->SelectedItem);
		Data::MyPostMessage(MyWindowMessage::SelectActor, (void*)int::Parse(text->Text));

	}
	void SetSelectItemReturnPoint(){
		if (!m_ActorIntPtrDataBox)return;

		m_ActorIntPtrDataBox->Text = "0";

		auto bind = gcnew System::Windows::Data::Binding("SelectedValue.DataPtr");
		bind->Source = m_TreeView;
		//�X�V�C�x���g���Ă�
		bind->NotifyOnTargetUpdated = true;
		m_ActorIntPtrDataBox->SetBinding(TextBlock::TextProperty, bind);
		//�X�V�C�x���g
		m_ActorIntPtrDataBox->TargetUpdated += gcnew System::EventHandler<System::Windows::Data::DataTransferEventArgs^>(this, &View::ActorIntPtr_TargetUpdated);
		//�A�N�^�[��IntPtrViewModel���o�C���h
		//m_ActorIntPtrDataBox->DataContext = vm;


	}
	void ChangeTreeViewName(String^ name, IntPtr treeviewptr){
		auto i = (gcroot<TestContent::Person^>*)(void*)treeviewptr;
		(*i)->Name = name;
	}

private:
	//�A�Z�b�g�c���[�r���[�쐬
	void CreateAssetTreeView(System::Windows::Controls::Decorator ^dec){
		//�c���[�r���[�쐬
		auto sp = (Panel^)LoadContentsFromResource(IDR_TREEVIEW);
		dec->Child = sp;
		auto treeView = (TreeView^)sp->FindName("treeView1");

		//�A�C�e�����X�g�쐬
		auto list = gcnew TestContent::MyList();
		//�A�C�e�����X�g�̃��[�g���쐬
		auto root = gcnew TestContent::Person("root", list);
		treeView->DataContext = root;
		auto source = gcnew System::Windows::Data::Binding("Children");
		source->Mode = System::Windows::Data::BindingMode::TwoWay;
		treeView->SetBinding(TreeView::ItemsSourceProperty, source);


		//�A�C�e�����X�g�̃f�[�^�\��
		auto datatemp = gcnew System::Windows::HierarchicalDataTemplate();
		//�A�C�e���̃��X�g�o�C���h
		auto datasource = gcnew System::Windows::Data::Binding("Children");
		datasource->Mode = System::Windows::Data::BindingMode::TwoWay;
		datatemp->ItemsSource = datasource;

		//�ǉ�����A�C�e���̃R���g���[��
		auto fact = gcnew System::Windows::FrameworkElementFactory();
		fact->Type = TextBlock::typeid;
		auto itembind = gcnew System::Windows::Data::Binding("Name");
		itembind->Mode = BindingMode::TwoWay;
		fact->SetBinding(TextBlock::TextProperty, itembind);
		fact->AddHandler(TextBlock::MouseDownEvent,gcnew MouseButtonEventHandler(this,&View::OnMouseDown));
		datatemp->VisualTree = fact;

		treeView->ItemTemplate = datatemp;

		treeView->AllowDrop = true;
		treeView->Drop += gcnew DragEventHandler(this, &View::OnDrop);
		treeView->DragEnter += gcnew DragEventHandler(this, &View::OnDragOver2);
		treeView->DragOver += gcnew DragEventHandler(this, &View::OnDragOver2);

		//�I�𒆂̃A�C�e���\��
		//auto tblock = (TextBlock ^)sp->FindName("textBlock1");
		//m_ActorIntPtrDataBox = tblock;

		auto dirs = gcnew System::IO::DirectoryInfo("Assets/");

		auto e = dirs->EnumerateFiles()->GetEnumerator();
		while(e->MoveNext()){
			auto item = gcnew TestContent::Person("Assets/" + e->Current->Name, gcnew TestContent::MyList());
			item->DataPtr = (IntPtr)NULL;
			root->Add(item);
		}

	}

	void OnMouseDown(Object ^s, MouseButtonEventArgs ^e)
	{
		//if (m_GameScreen->m_wfh->Visibility == System::Windows::Visibility::Visible)
		//	m_GameScreen->m_wfh->Visibility = System::Windows::Visibility::Collapsed;
		//else
		//	m_GameScreen->m_wfh->Visibility = System::Windows::Visibility::Visible;
		//System::Windows::MessageBox::Show("borderClick");
		auto item = (TextBlock^)s;
		if (e->LeftButton == MouseButtonState::Pressed
			&& e->RightButton == MouseButtonState::Released
			&& e->MiddleButton == MouseButtonState::Released)
			DragDrop::DoDragDrop(
			item, // �h���b�O����镨
			item, // �n���f�[�^
			DragDropEffects::Copy); // D&D�ŋ�����I�y���[�V����

	}
	void OnDrop(Object ^s, DragEventArgs ^e)
	{
		//System::Windows::MessageBox::Show(e->Data->GetData(TextBlock::typeid)->ToString() + " is dropped.", "Dropped!");
	}

	void OnDragOver(Object ^sender, DragEventArgs ^e)
	{
		// �h���b�v�����f�[�^��String�łȂ���Ύ󂯓���Ȃ�
		if (!e->Data->GetDataPresent(TextBlock::typeid))
		{
			e->Effects = DragDropEffects::None;
			return;
		}

		e->Effects = DragDropEffects::None;
	}
	void OnDragOver2(Object ^sender, DragEventArgs ^e)
	{
		// �h���b�v�����f�[�^��String�łȂ���Ύ󂯓���Ȃ�
		if (!e->Data->GetDataPresent(TextBlock::typeid))
		{
			e->Effects = DragDropEffects::None;
			return;
		}
		auto t = (TextBlock^)e->Data->GetData(TextBlock::typeid);
		if (t->Text->Contains(".prefab"))
			e->Effects = DragDropEffects::Copy;
		else
			e->Effects = DragDropEffects::None;
	}

	//�c���[�r���[�쐬
	void CreateTreeView(System::Windows::Controls::Decorator ^dec){
		//�c���[�r���[�쐬
		auto sp = (Panel^)LoadContentsFromResource(IDR_TREEVIEW);
		dec->Child = sp;
		auto treeView = (TreeView^)sp->FindName("treeView1");
		m_TreeView = treeView;

		//�A�C�e�����X�g�쐬
		auto list = gcnew TestContent::MyList();//gcnew List<TestContent::Person^>();
		//list->list->Add(gcnew TestContent::Person
		//	("U", gcnew TestContent::MyList(
		//			gcnew TestContent::Person("UNK", gcnew TestContent::MyList())
		//	))
		//);
		//�A�C�e�����X�g�̃��[�g���쐬
		auto root = gcnew TestContent::Person("root", list);
		treeView->DataContext = root;
		auto source = gcnew System::Windows::Data::Binding("Children");
		source->Mode = System::Windows::Data::BindingMode::TwoWay;
		treeView->SetBinding(TreeView::ItemsSourceProperty, source);
		m_TreeViewItemRoot = root;


		//�A�C�e�����X�g�̃f�[�^�\��
		auto datatemp = gcnew System::Windows::HierarchicalDataTemplate();
		//�A�C�e���̃��X�g�o�C���h
		auto datasource = gcnew System::Windows::Data::Binding("Children");
		datasource->Mode = System::Windows::Data::BindingMode::TwoWay;
		datatemp->ItemsSource = datasource;

		//�ǉ�����A�C�e���̃R���g���[��
		auto fact = gcnew System::Windows::FrameworkElementFactory();
		fact->Type = TextBlock::typeid;
		auto itembind = gcnew System::Windows::Data::Binding("Name");
		itembind->Mode = BindingMode::TwoWay;
		fact->SetBinding(TextBlock::TextProperty, itembind);
		datatemp->VisualTree = fact;

		treeView->ItemTemplate = datatemp;

		//�I�𒆂̃A�C�e���\��
		auto tblock = (TextBlock ^)sp->FindName("textBlock1");
		m_ActorIntPtrDataBox = tblock;


		//�R���e�L�X�g���j���[�쐬
		auto cm = gcnew System::Windows::Controls::ContextMenu();
		{//�R���e�L�X�g���j���[�̗v�f�쐬
			auto mitem = gcnew System::Windows::Controls::MenuItem();
			mitem->Header = "����";
			mitem->Click += gcnew System::Windows::RoutedEventHandler(this, &View::MenuItem_Click);
			cm->Items->Add(mitem);
		}
		{//�R���e�L�X�g���j���[�̗v�f�쐬
			auto mitem = gcnew System::Windows::Controls::MenuItem();
			mitem->Header = "AddScriptComponent";
			mitem->Click += gcnew System::Windows::RoutedEventHandler(this, &View::MenuItem_Click_AddScriptComponent);
			cm->Items->Add(mitem);
		}
		{//�R���e�L�X�g���j���[�̗v�f�쐬
			auto mitem = gcnew System::Windows::Controls::MenuItem();
			mitem->Header = "RemoveScriptComponent";
			mitem->Click += gcnew System::Windows::RoutedEventHandler(this, &View::MenuItem_Click_RemoveScriptComponent);
			cm->Items->Add(mitem);
		}
		{//�R���e�L�X�g���j���[�̗v�f�쐬
			auto mitem = gcnew System::Windows::Controls::MenuItem();
			mitem->Header = "AddPhysXComponent";
			mitem->Click += gcnew System::Windows::RoutedEventHandler(this, &View::MenuItem_Click_AddPhysXComponent);
			cm->Items->Add(mitem);
		}
		{//�R���e�L�X�g���j���[�̗v�f�쐬
			auto mitem = gcnew System::Windows::Controls::MenuItem();
			mitem->Header = "RemovePhysXComponent";
			mitem->Click += gcnew System::Windows::RoutedEventHandler(this, &View::MenuItem_Click_RemovePhysXComponent);
			cm->Items->Add(mitem);
		}
		{//�R���e�L�X�g���j���[�̗v�f�쐬
			auto mitem = gcnew System::Windows::Controls::MenuItem();
			mitem->Header = "AddPhysXColliderComponent";
			mitem->Click += gcnew System::Windows::RoutedEventHandler(this, &View::MenuItem_Click_AddPhysXColliderComponent);
			cm->Items->Add(mitem);
		}
		{//�R���e�L�X�g���j���[�̗v�f�쐬
			auto mitem = gcnew System::Windows::Controls::MenuItem();
			mitem->Header = "RemovePhysXColliderComponent";
			mitem->Click += gcnew System::Windows::RoutedEventHandler(this, &View::MenuItem_Click_RemovePhysXColliderComponent);
			cm->Items->Add(mitem);
		}
		//�c���[�r���[�ɔ��f
		m_TreeView->ContextMenu = cm;

	}

	void OnDrag(Object ^sender, System::Windows::RoutedEventArgs ^e){
		//auto item = (TestContent::Person^)e->Source;
		//System::Windows::MessageBox::Show("had");
		auto tv = (TreeView^)sender;
		//tv->Background = Brush
		System::Windows::MessageBox::Show(e->Source->ToString());
		e->Handled = true;
		//tv->Cursor = gcnew System::Windows::Input::Cursor("Hand");
		//System::Windows::MessageBox::Show("had");
	}
	void MenuItem_Click(Object ^sender, System::Windows::RoutedEventArgs ^e){
			{
				if (m_TreeView->SelectedItem == nullptr)return;
				auto i = (TestContent::Person^)m_TreeView->SelectedItem;
				//i->Name = "����";
				//m_TreeViewItemRoot->Children->Remove(i);
				Data::MyPostMessage(MyWindowMessage::ActorDestroy, (void*)i->DataPtr);
				//((IntViewModel^)m_ActorIntPtrDataBox->DataContext)->Value = "0";
				//m_ActorIntPtrDataBox->Text = "0";
				//((TestContent::Person^)m_TreeView->SelectedItem)->DataPtr = (IntPtr)0;

				i->RemoveSelf();

				//�v�f������Ƃ��̂�
				if (m_TreeViewItemRoot->Children->Count != 0){
					//�Z���N�g������
					ActorIntPtr_TargetUpdated(m_ActorIntPtrDataBox, nullptr);
				}

				e->Handled = true;
			}
	}

#pragma region MenuContext
	void PostMessageRemoveComponent(const char* t){
		if (m_TreeView->SelectedItem == nullptr)return;
		Data::MyPostMessage(MyWindowMessage::RemoveComponent, (void*)t);
	}
	void PostMessageAddComponent(const char* t){
		if (m_TreeView->SelectedItem == nullptr)return;
		Data::MyPostMessage(MyWindowMessage::AddComponent, (void*)t);
	}
	void MenuItem_Click_AddScriptComponent(Object ^sender, System::Windows::RoutedEventArgs ^e){
		PostMessageAddComponent("Script");
		e->Handled = true;
	}
	void MenuItem_Click_RemoveScriptComponent(Object ^sender, System::Windows::RoutedEventArgs ^e){
		PostMessageRemoveComponent("Script");
		e->Handled = true;
	}

	void MenuItem_Click_AddPhysXComponent(Object ^sender, System::Windows::RoutedEventArgs ^e){
		PostMessageAddComponent("PhysX");
		e->Handled = true;
	}
	void MenuItem_Click_RemovePhysXComponent(Object ^sender, System::Windows::RoutedEventArgs ^e){
		PostMessageRemoveComponent("PhysX");
		e->Handled = true;
	}

	void MenuItem_Click_AddPhysXColliderComponent(Object ^sender, System::Windows::RoutedEventArgs ^e){
		PostMessageAddComponent("Collider");
		e->Handled = true;
	}
	void MenuItem_Click_RemovePhysXColliderComponent(Object ^sender, System::Windows::RoutedEventArgs ^e){
		PostMessageRemoveComponent("Collider");
		e->Handled = true;
	}
//MenuContext
#pragma endregion 

protected:
	void OnSourceInitialized(System::EventArgs ^e) override
	{
		Window::OnSourceInitialized(e);
		mGameScreenHWND = m_GameScreen->GetHWND();

	}

public:
	property GameScreen^ GameScreenData{
		GameScreen^ get(){
			return m_GameScreen;
		}
	}
private:

	GameScreen ^m_GameScreen;
	HWND mGameScreenHWND;

	StackPanel ^m_ComponentPanel;

	TreeView ^ m_TreeView;
	TestContent::Person ^m_TreeViewItemRoot;
	TextBlock ^m_ActorIntPtrDataBox;

public:
	property HWND GameScreenHWND{
		HWND get(){
			return mGameScreenHWND;
		}
	}

};