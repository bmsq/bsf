#include "BsScriptGUIVector4Field.h"
#include "BsScriptMeta.h"
#include "BsMonoField.h"
#include "BsMonoClass.h"
#include "BsMonoManager.h"
#include "BsMonoMethod.h"
#include "BsSpriteTexture.h"
#include "BsMonoUtil.h"
#include "BsGUILayout.h"
#include "BsGUIVector4Field.h"
#include "BsGUIOptions.h"
#include "BsGUIContent.h"
#include "BsScriptGUIElementStyle.h"
#include "BsScriptGUILayout.h"
#include "BsScriptGUIArea.h"
#include "BsScriptHString.h"
#include "BsScriptGUIContent.h"

using namespace std::placeholders;

namespace BansheeEngine
{
	ScriptGUIVector4Field::OnChangedThunkDef ScriptGUIVector4Field::onChangedThunk;

	ScriptGUIVector4Field::ScriptGUIVector4Field(MonoObject* instance, GUIVector4Field* vector4Field)
		:TScriptGUIElement(instance, vector4Field)
	{

	}

	void ScriptGUIVector4Field::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_CreateInstance", &ScriptGUIVector4Field::internal_createInstance);
		metaData.scriptClass->addInternalCall("Internal_GetValue", &ScriptGUIVector4Field::internal_getValue);
		metaData.scriptClass->addInternalCall("Internal_SetValue", &ScriptGUIVector4Field::internal_setValue);
		metaData.scriptClass->addInternalCall("Internal_HasInputFocus", &ScriptGUIVector4Field::internal_hasInputFocus);

		onChangedThunk = (OnChangedThunkDef)metaData.scriptClass->getMethod("DoOnChanged", 1)->getThunk();
	}

	void ScriptGUIVector4Field::internal_createInstance(MonoObject* instance, MonoObject* title, UINT32 titleWidth,
		MonoString* style, MonoArray* guiOptions, bool withTitle)
	{
		GUIOptions options;

		UINT32 arrayLen = (UINT32)mono_array_length(guiOptions);
		for (UINT32 i = 0; i < arrayLen; i++)
			options.addOption(mono_array_get(guiOptions, GUIOption, i));

		String styleName = toString(MonoUtil::monoToWString(style));

		GUIVector4Field* field = nullptr;
		if (withTitle)
		{
			GUIContent nativeContent(ScriptGUIContent::getText(title), ScriptGUIContent::getImage(title), ScriptGUIContent::getTooltip(title));
			field = GUIVector4Field::create(nativeContent, titleWidth, options, styleName);
		}
		else
		{
			field = GUIVector4Field::create(options, styleName);
		}

		field->onValueChanged.connect(std::bind(&ScriptGUIVector4Field::onChanged, instance, _1));

		ScriptGUIVector4Field* nativeInstance = new (bs_alloc<ScriptGUIVector4Field>()) ScriptGUIVector4Field(instance, field);
	}

	void ScriptGUIVector4Field::internal_getValue(ScriptGUIVector4Field* nativeInstance, Vector4* output)
	{
		GUIVector4Field* field = static_cast<GUIVector4Field*>(nativeInstance->getGUIElement());

		*output = field->getValue();
	}

	void ScriptGUIVector4Field::internal_setValue(ScriptGUIVector4Field* nativeInstance, Vector4 value)
	{
		GUIVector4Field* field = static_cast<GUIVector4Field*>(nativeInstance->getGUIElement());

		return field->setValue(value);
	}

	void ScriptGUIVector4Field::internal_hasInputFocus(ScriptGUIVector4Field* nativeInstance, bool* output)
	{
		GUIVector4Field* field = static_cast<GUIVector4Field*>(nativeInstance->getGUIElement());

		*output = field->hasInputFocus();
	}

	void ScriptGUIVector4Field::onChanged(MonoObject* instance, Vector4 newValue)
	{
		MonoException* exception = nullptr;
		onChangedThunk(instance, newValue, &exception);

		MonoUtil::throwIfException(exception);
	}
}