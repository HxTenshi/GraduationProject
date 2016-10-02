#pragma once

class Inspector;
class I_ioHelper;
class Enabled{
public:
	Enabled()
		: m_IsEnabled(true)
		, m_IsParentEnabled(true)
	{}

	virtual ~Enabled(){}

	//�L����
	void Enable(){
		_TryChange(true, m_IsParentEnabled);
	}
	//������
	void Disable(){
		_TryChange(false, m_IsParentEnabled);
	}
	//�L�����H
	bool IsEnabled(){
		return m_IsEnabled && m_IsParentEnabled;
	}
	//�L�����H
	bool IsLocalEnabled(){
		return m_IsEnabled;
	}

protected:
	//�R�[���o�b�N�p
	void ChildEnableChanged(Enabled* child){
		bool current = m_IsEnabled && m_IsParentEnabled;
		child->_TryChange(child->IsLocalEnabled(), current);
	}

	void IO_Data(I_ioHelper* io);
private:
	virtual void OnEnabled(){}
	virtual void OnDisabled(){}

	void _TryChange(bool enable,bool parent_enable){
		//�ύX�̊m�F
		if (m_IsEnabled == enable && m_IsParentEnabled == parent_enable)return;
		//�R�[���o�b�N�̏󋵊m�F�p
		bool current = m_IsEnabled && m_IsParentEnabled;
		bool next = enable && parent_enable;
		//�p�����[�^�[�̃Z�b�g
		m_IsEnabled = enable;
		m_IsParentEnabled = parent_enable;
		//�R�[���o�b�N���ĂԕK�v���Ȃ����
		if (current == next)return;
		//�R�[���o�b�N
		next?
			OnEnabled():
			OnDisabled();
	}

	bool m_IsEnabled;
	bool m_IsParentEnabled;

	friend Inspector;
};