#pragma once

#include "InspectorDataSet.h"

void OnKeyDownHandler(Object ^sender, System::Windows::Input::KeyEventArgs ^e)
{
	if (e->Key == System::Windows::Input::Key::Return)
	{
		auto t = (TextBox^)sender;
		auto sp = (Panel^)t->Parent;
		sp->Focusable = true;
		sp->Focus();
		sp->Focusable = false;
		//t->MoveFocus(gcnew System::Windows::Input::TraversalRequest(System::Windows::Input::FocusNavigationDirection::Previous));
	}
}
void TextBoxFocused(Object^ sender, System::Windows::DependencyPropertyChangedEventArgs e){
	if ((*(bool^)e.NewValue)){
		((IViewModel^)((TextBox^)sender)->DataContext)->UpdateView();
		((IViewModel^)((TextBox^)sender)->DataContext)->UpdateViewFlag = false;
	}
	else
		((IViewModel^)((TextBox^)sender)->DataContext)->UpdateViewFlag = true;

}


template <class T>
void BindingInspector(TextBox^ textbox, T* pf, std::function<void(T)> collback){
	auto vm = gcnew ViewModel<T>(pf, collback);
	textbox->DataContext = vm;
	textbox->SetBinding(TextBox::TextProperty, "Value");
	textbox->IsKeyboardFocusedChanged += gcnew System::Windows::DependencyPropertyChangedEventHandler(&TextBoxFocused);
	textbox->KeyDown += gcnew System::Windows::Input::KeyEventHandler(&OnKeyDownHandler);
}

template <class T>
void BindingInspector(CheckBox^ textbox, T* pf, std::function<void(T)> collback){
	auto vm = gcnew ViewModel<T>(pf, collback);
	textbox->DataContext = vm;
	textbox->SetBinding(CheckBox::IsCheckedProperty, "Value");
}

template <class T>
void BindingInspector(Slider^ slide, T min, T max, T* pf, std::function<void(T)> collback){
	auto vm = gcnew ViewModel<T>(pf, collback);
	slide->DataContext = vm;
	slide->SetBinding(Slider::ValueProperty, "Value");
	slide->Minimum = min;
	slide->Maximum = max;
}

//���\�[�X����XAML���쐬����
FrameworkElement ^LoadContentsFromResource(int resourceId) {
	// ���W���[���n���h���̎擾
	HMODULE module = GetModuleHandle(L"CppWPFdll");
	// ���\�[�X��������
	HRSRC resource = FindResource(module, MAKEINTRESOURCE(resourceId), RT_HTML);
	// ���\�[�X�̃f�[�^�T�C�Y���擾����
	DWORD dataSize = SizeofResource(module, resource);
	// ���\�[�X�f�[�^�̃O���[�o���n���h�����擾����
	HGLOBAL dataHandle = LoadResource(module, resource);
	// ���\�[�X�̃f�[�^���擾����
	const char *data = static_cast<const char *>(LockResource(dataHandle));

	// ���\�[�X���Ōオ�k�������̕�����ɂ��邽�߂Ƀo�b�t�@�[�ɃR�s�[����
	std::vector<char> xaml(dataSize + 1);
	std::copy(data, data + dataSize, xaml.begin());

	// ���\�[�X�̉��
	UnlockResource(dataHandle);
	FreeResource(dataHandle);

	// XAML����I�u�W�F�N�g��
	StringReader ^reader = gcnew StringReader(gcnew String(xaml.data()));
	XamlXmlReader ^xamlXmlReader = gcnew XamlXmlReader(reader);
	XamlObjectWriter ^xamlObjectWriter = gcnew XamlObjectWriter(xamlXmlReader->SchemaContext);
	while (xamlXmlReader->Read()) {
		xamlObjectWriter->WriteNode(xamlXmlReader);
	}
	return (FrameworkElement ^)xamlObjectWriter->Result;
}

//template <class T>
//FrameworkElement^ _GetFrom(){
//	return LoadContentsFromResource(IDR_INS_FLOAT);
//}
//template <>
//FrameworkElement^ _GetFrom<bool>(){
//	return LoadContentsFromResource(IDR_INS_CHECKBOX);
//}


template <class T>
void _CreateInspector(DockPanel^ dockPanel, String^ text, T* p, std::function<void(T)> collback){
	FrameworkElement ^com = LoadContentsFromResource(IDR_INS_FLOAT);
	dockPanel->Children->Add(com);
	DockPanel::SetDock(com, System::Windows::Controls::Dock::Top);
	auto tb = (TextBlock^)com->FindName("FloatName");
	if (tb)tb->Text = text;
	BindingInspector<T>((TextBox^)com->FindName("Value"), p, collback);

}




template <class T>
ref class Inspector : public InspectorData{
public:
	Inspector(TemplateInspectorDataSet<T>* data)
		:m_data(data){
	}
	~Inspector(){
		this->!Inspector();
	}
	!Inspector(){
		delete m_data;
	}

	void CreateInspector(DockPanel^ dockPanel) override{
		_CreateInspector<T>(dockPanel, gcnew String(m_data->Text.c_str()), m_data->data, m_data->collBack);
	}
private:
	TemplateInspectorDataSet<T> *m_data;
};