#pragma once
#include <Windows.h>

#define WM_MYWMCOLLBACK		(WM_APP+0x0001)
//WPF�E�B���h�E���瑗���郁�b�Z�[�W���`
enum class MyWindowMessage{
	StackIntPtr,			//IntPtr���X�^�b�N����
	ReturnTreeViewIntPtr,	//�c���[�r���[��IntPtr��n��
	ScriptCompile,
	SaveScene,

	SelectAsset,
	OpenAsset,

	CreateScriptFile,
	CreateAssetFile,

	//�t�@�C������n
	AssetFileChanged,
	AssetFileCreated,
	AssetFileDeleted,
	AssetFileRenamed_OldName,
	AssetFileRenamed_NewName,

	PlayGame,
	StopGame,
	CreateModelConvert,
	CreatePrefabToActor,
	CreateActorToPrefab,

	SelectActor,
	SetActorParent,
	ActorDoubleClick,
	ActorDestroy,
	ChangeParamComponent,
	AddComponent,
	RemoveComponent,
	Count,
};