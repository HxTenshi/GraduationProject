#include "OutputGimic.h"

#include "OutputStartGimic.h"
#include "OutputConnectorOneCollGimic.h"
#include "OutputConnectorMultipleExporterGimic.h"
#include "OutputChangeEnableGimic.h"
#include "OutputConectorRandomGimic.h"
#include "OutputMoveObjectGimic.h"


OutputGimic * OutputGimic::GetOutputGimic(GameObject target)
{
	if (!target)return NULL;

	if (auto scr = target->GetScript<OutputStartGimic>())return scr;
	if (auto scr = target->GetScript<OutputConnectorOneCollGimic>())return scr;
	if (auto scr = target->GetScript<OutputConnectorMultipleExporterGimic>())return scr;
	if (auto scr = target->GetScript<OutputChangeEnableGimic>())return scr;
	if (auto scr = target->GetScript<OutputConectorRandomGimic>())return scr;
	if (auto scr = target->GetScript<OutputMoveObjectGimic>())return scr;
	

	return NULL;
}
