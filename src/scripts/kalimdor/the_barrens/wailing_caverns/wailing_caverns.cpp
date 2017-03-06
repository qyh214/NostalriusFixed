/*
 * Scripted for --> Mangos-Zero Special Thanks for VladimirMangos, Yehonal, Theluda, Drkotas, Shin, Wrath Team.
 * Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Wailing Caverns
SD%Complete:
SDComment:
SDCategory: Wailing Caverns
EndScriptData */

/* ContentData
npc_disciple_of_naralex
EndContentData */

#include "scriptPCH.h"
#include "def_wailing_caverns.h"

/*######
##npc_disciple_of_naralex
######*/

enum
{
    SPELL_MARK              = 5232,
    SPELL_SLEEP             = 1090,
    SPELL_POTION            = 8141,
    SPELL_CLEANSING         = 6270,
    SPELL_AWAKENING         = 6271,
    SPELL_SHAPESHIFT        = 8153,

    //YELL_AFTER_GOSSIP       = -1614999,
    SAY_CAST_MARK           = -1614998,
    SAY_1ST_WP              = -1614997,
    SAY_AFTER_1ST_TRASH     = -1614996,
    SAY_BEFORE_CIRCLE       = -1614995,
    SAY_AFTER_CIRCLE        = -1614994,
    SAY_BEFORE_CHAMBER      = -1614993,
    SAY_BEFORE_RITUAL       = -1614992,
    EMOTE_DISCIPLE_1        = -1614991,
    EMOTE_NARALEX_1         = -1614990,
    SAY_ATTACKED            = -1614989,
    EMOTE_NARALEX_2         = -1614987,
    SAY_BEFORE_MUTANOUS     = -1614986,
    SAY_NARALEX_AWAKEN      = -1614985,
    SAY_DISCIPLE_FINAL      = -1614984,
    SAY_NARALEX_FINAL1      = -1614983,
    SAY_NARALEX_FINAL2      = -1614982,

    MOB_DEVIATE_RAPTOR      = 3636,
    MOB_DEVIATE_VIPER       = 5755,
    MOB_DEVIATE_MOCCASIN    = 5762,
    MOB_NIGHTMARE_ECTOPLASM = 5763,
    MOB_MUTANOUS_DEVOURER   = 3654,

    POINT_LAST_POINT    = 0xFFFFFF
};

#define GOSSIP_ITEM_BEGIN   "Let the event begin!"

float Position [10][3] =
{
    { -33.5f, 243.3f, -93.6f},
    { -61.5f, 296.6f, -89.8f},
    { -70.3f, 259.7f, -91.8f},
    {142.7f, 254.0f, -102.2f},
    {140.5f, 219.8f, -102.4f},
    {92.2f, 261.9f, -101.5f},
    {100.3f, 268.6f, -102.2f},
    {123.8f, 271.9f, -102.4f},
    {141.3f, 255.1f, -102.2f},
    {142.4f, 199.7f, -102.2f}
};

struct npc_disciple_of_naralexAI : public npc_escortAI
{
    npc_disciple_of_naralexAI(Creature* pCreature) : npc_escortAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Yelled = false;
        isAggro = false;
        Reset();
    }

    ScriptedInstance* m_pInstance;
    ObjectGuid m_playerGuid;

    uint32 Event_Timer;
    uint32 Sleep_Timer;
    uint32 Potion_Timer;
    uint32 Cleansing_Timer;

    int32 Point;
    int8 Subevent_Phase;

    bool Yelled;
    bool isAggro;

    void Reset()
    {
        Sleep_Timer = 5000;
        Potion_Timer = 5000;
        Cleansing_Timer = 0;
        Yelled = false;

        if (isAggro)
        {
            SetEscortPaused(false);
            //m_creature->GetMotionMaster()->MoveWaypoint();
            float fPosX, fPosY, fPosZ;
            m_creature->GetCombatStartPosition(fPosX, fPosY, fPosZ);
            m_creature->GetMotionMaster()->MovePoint(POINT_LAST_POINT, fPosX, fPosY, fPosZ);
            m_creature->SetWalk(true);
            isAggro = false;
        }
    }

    void EnterEvadeMode()
    {
        Reset();

        if (Point == 15 || Point == 30)
        {
            m_creature->DeleteThreatList();
            m_creature->CombatStop(true);
            Reset();
        }
        else
            npc_escortAI::EnterEvadeMode();
    }

    void Aggro(Unit* pWho)
    {
        isAggro = true;
        SetEscortPaused(true);
    }

    void WaypointReached(uint32 i)
    {
        switch (i)
        {
            case 0:
                DoScriptText(SAY_CAST_MARK, m_creature);
                m_creature->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                SetEscortPaused(true);
                Event_Timer = 5000;
                Point = i;
                break;
            case 7:
                DoScriptText(SAY_1ST_WP, m_creature);
                m_creature->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                Event_Timer = 7000;
                Point = i;
                SetEscortPaused(true);
                break;
            case 8:
                DoScriptText(SAY_AFTER_1ST_TRASH, m_creature);
                m_creature->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                break;
            case 15:
                Subevent_Phase = 0;
                Event_Timer = 2000;
                Point = i;
                SetEscortPaused(true);
                break;
            case 26:
                DoScriptText(SAY_BEFORE_CHAMBER, m_creature);
                m_creature->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                break;
            case 30:
                m_pInstance->SetData(TYPE_DISCIPLE, IN_PROGRESS);
                Event_Timer = 1000;
                Subevent_Phase = 0;
                Point = i;
                SetEscortPaused(true);
            default:
                break;
        }
    }

    void SummonAttacker(uint32 entry, float x, float y, float z)
    {
        Creature* Summoned = m_creature->SummonCreature(entry, x, y, z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 40000);//20000
        if (!Summoned)
            return;
        Player* eventStarter = m_creature->GetMap()->GetPlayer(m_playerGuid);
        if (eventStarter)
            Summoned->AI()->AttackStart(eventStarter);
        else
            Summoned->GetMotionMaster()->MovePoint(1, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), MOVE_PATHFINDING);
    }

    void UpdateEscortAI(const uint32 diff)
    {
        if (!m_pInstance)
            return;

        Unit* Naralex = m_creature->GetMap()->GetUnit(m_pInstance->GetData64(DATA_NARALEX));
        if (!Naralex)
            return;

        if (Event_Timer && Event_Timer <= diff)
        {
            if (!m_creature->IsWalking())
            {
                SetRun(false);
                m_creature->SetWalk(true);
            }

            switch (Point)
            {
                case 0:
                {
                    Map *map = m_creature->GetMap();
                    if (map && map->IsDungeon())
                    {
                        Map::PlayerList const &PlayerList = map->GetPlayers();
                        if (!PlayerList.isEmpty())
                        {
                            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                            {
                                if (i->getSource()->isAlive() && (i->getSource()->GetDistance(m_creature) < 30))
                                    m_creature->CastSpell(i->getSource(), SPELL_MARK, false);
                            }
                        }
                    }
                    SetEscortPaused(false);
                    Event_Timer = 0;
                    break;
                }

                case 7:
                    SetEscortPaused(false);
                    Event_Timer = 0;
                    SummonAttacker(MOB_DEVIATE_RAPTOR, -67.851196f, 214.383102f, -93.499001f);
                    SummonAttacker(MOB_DEVIATE_RAPTOR, -69.769707f, 211.342804f, -93.450737f);

                    break;

                case 15:
                    switch (Subevent_Phase)
                    {
                        case 0:
                            DoScriptText(SAY_BEFORE_CIRCLE, m_creature);
                            m_creature->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                            Subevent_Phase = 1;
                            Event_Timer = 2000;
                            break;
                        case 1:
                            m_creature->CastSpell(m_creature, SPELL_CLEANSING, false);
                            Subevent_Phase = 2;
                            Event_Timer = 30000;
                            break;
                        case 2:
                            for (int i = 0; i < 3; ++i)
                                SummonAttacker(MOB_DEVIATE_VIPER, Position[i][0], Position[i][1], Position[i][2]);
                            Subevent_Phase = 3;
                            Event_Timer = 2000;
                            break;
                        case 3:
                        {
                            Player* eventStarter = m_creature->GetMap()->GetPlayer(m_playerGuid);
                            if (!m_creature->isInCombat() && eventStarter && !eventStarter->isInCombat())
                            {
                                DoScriptText(SAY_AFTER_CIRCLE, m_creature);
                                m_creature->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                                Point = 16;
                                Event_Timer = 2000;
                                SetEscortPaused(false);
                                break;
                            }
                            else
                            {
                                Event_Timer = 2000;
                                Subevent_Phase = 3;
                                break;
                            }
                        }
                    }
                    break;
                case 30:
                    SetEscortPaused(true);
                    switch (Subevent_Phase)
                    {
                        case 0:
                            DoScriptText(SAY_BEFORE_RITUAL, m_creature);
                            m_creature->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                            Subevent_Phase = 1;
                            Event_Timer = 2000;
                            break;
                        case 1:
                            m_creature->CastSpell(m_creature, SPELL_AWAKENING, false);
                            DoScriptText(EMOTE_DISCIPLE_1, m_creature);
                            m_creature->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                            Subevent_Phase = 2;
                            Event_Timer = 4000;
                            break;
                        case 2:
                            DoScriptText(EMOTE_NARALEX_1, Naralex);
                            Naralex->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                            Subevent_Phase = 3;
                            Event_Timer = 5000;
                            break;
                        case 3:
                            for (int i = 3; i < 6; ++i)
                                SummonAttacker(MOB_DEVIATE_MOCCASIN, Position[i][0], Position[i][1], Position[i][2]);
                            Event_Timer = 40000;
                            Subevent_Phase = 4;
                            break;
                        case 4:
                            for (int i = 3; i < 10; ++i)
                                SummonAttacker(MOB_NIGHTMARE_ECTOPLASM, Position[i][0], Position[i][1], Position[i][2]);
                            Event_Timer = 40000;
                            Subevent_Phase = 5;
                            break;
                        case 5:
                            Subevent_Phase = 6;
                            Event_Timer = 10000;
                            DoScriptText(EMOTE_NARALEX_2, Naralex);
                            Naralex->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                            DoScriptText(SAY_BEFORE_MUTANOUS, m_creature);
                            m_creature->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                            break;
                        case 6:
                            Creature* Mutanous2;
                            if (Mutanous2 = m_creature->SummonCreature(MOB_MUTANOUS_DEVOURER, Position[4][0], Position[4][1], Position[4][2], 0, TEMPSUMMON_MANUAL_DESPAWN))
                            {
                                Mutanous2->SetRespawnDelay(86400000);
                                if (Player* eventStarter = m_creature->GetMap()->GetPlayer(m_playerGuid))
                                    Mutanous2->AI()->AttackStart(eventStarter);
                            }
                            Subevent_Phase = 7;
                            Event_Timer = 2000;
                            break;
                        case 7:
                            if (m_pInstance->GetData(TYPE_MUTANOUS) == DONE)
                            {
                                Naralex->SetByteValue(UNIT_FIELD_BYTES_1, 0, UNIT_STAND_STATE_SIT);
                                DoScriptText(SAY_NARALEX_AWAKEN, Naralex);
                                Naralex->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                                m_creature->InterruptNonMeleeSpells(false, SPELL_AWAKENING);
                                m_creature->RemoveAurasDueToSpell(SPELL_AWAKENING);
                                m_pInstance->SetData(TYPE_DISCIPLE, DONE);
                                Event_Timer = 2000;
                                Subevent_Phase = 8;
                                break;
                            }
                            else
                            {
                                Event_Timer = 2000;
                                Subevent_Phase = 7;
                                break;
                            }
                        case 8:
                            DoScriptText(SAY_DISCIPLE_FINAL, m_creature);
                            m_creature->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                            Subevent_Phase = 9;
                            Event_Timer = 5000;
                            break;
                        case 9:
                            DoScriptText(SAY_NARALEX_FINAL1, Naralex);
                            Naralex->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                            Naralex->SetByteValue(UNIT_FIELD_BYTES_1, 0, UNIT_STAND_STATE_STAND);
                            Event_Timer = 5000;
                            Subevent_Phase = 10;
                            break;
                        case 10:
                            DoScriptText(SAY_NARALEX_FINAL2, Naralex);
                            Naralex->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                            m_creature->CastSpell(m_creature, SPELL_SHAPESHIFT, false);
                            Naralex->CastSpell(Naralex, SPELL_SHAPESHIFT, false);
                            Event_Timer = 10000;
                            Subevent_Phase = 11;
                            break;
                        case 11:
                            //m_creature->MonsterMove(Position[9][0],Position[9][1],Position[9][2]);
                            //Naralex->MonsterMove(Position[9][0],Position[9][1],Position[9][2]);
                            m_creature->SetFly(true);
                            Naralex->SetFly(true);
                            m_creature->MonsterMoveWithSpeed(Position[9][0], Position[9][1], Position[9][2], 10, false, true);
                            Naralex->MonsterMoveWithSpeed(Position[9][0], Position[9][1], Position[9][2], 10, false, true);
                            Event_Timer = 5000;
                            Subevent_Phase = 12;
                            break;
                        case 12:
                            m_creature->SetVisibility(VISIBILITY_OFF);
                            Naralex->SetVisibility(VISIBILITY_OFF);
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        else Event_Timer -= diff;

        if (Potion_Timer < diff)
        {
            if ((m_creature->GetHealth() / (double)m_creature->GetMaxHealth()) < 0.8)
                m_creature->CastSpell(m_creature, SPELL_POTION, false);
            Potion_Timer = 45000;
        }
        else Potion_Timer -= diff;

        if (m_creature->SelectHostileTarget() && m_creature->getVictim())
        {
            if (!Yelled)
            {
                DoScriptText(SAY_ATTACKED, m_creature, m_creature->getVictim());
                Yelled = true;
            }
            if (Sleep_Timer < diff)
            {
                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                    m_creature->CastSpell(target, SPELL_SLEEP, false);
                Sleep_Timer = 30000;
            }
            else Sleep_Timer -= diff;

            DoMeleeAttackIfReady();
        }
        else if (isAggro)
            EnterEvadeMode();
    }
};

bool GossipHello_npc_disciple_of_naralex(Player* pPlayer, Creature* pCreature)
{
    ScriptedInstance* m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

    pCreature->CastSpell(pPlayer, SPELL_MARK, false);

    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());
    if (m_pInstance && m_pInstance->GetData(TYPE_DISCIPLE) == SPECIAL)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_BEGIN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        pPlayer->SEND_GOSSIP_MENU(699, pCreature->GetGUID());
    }
    else
        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_disciple_of_naralex(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    ScriptedInstance* m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

    if (!m_pInstance)
        return false;

    if (uiAction == GOSSIP_ACTION_INFO_DEF + 1)
    {
        if (npc_disciple_of_naralexAI* pEscortAI = dynamic_cast<npc_disciple_of_naralexAI*>(pCreature->AI()))
        {
            pEscortAI->Start(false, /*pPlayer->GetGUID()*/NULL);//we don't want the out of range check.
            pEscortAI->m_playerGuid = pPlayer->GetObjectGuid();
            pCreature->setFaction(pPlayer->getFaction());
            pCreature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            pCreature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
            pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP);
            //DoScriptText(YELL_AFTER_GOSSIP,pCreature);
        }
        pPlayer->CLOSE_GOSSIP_MENU();
    }
    return true;
}

CreatureAI* GetAI_npc_disciple_of_naralex(Creature* pCreature)
{
    return new npc_disciple_of_naralexAI(pCreature);
}

enum
{
    SPELL_IMMUNE_FIRE    =   7942,
    SPELL_IMMUNE_FROST   =   7940,
    SPELL_IMMUNE_NATURE  =   7941,
    SPELL_IMMUNE_SHADOW  =   7743,
};

struct EvolvingEctoplasmAI : public ScriptedAI
{
    EvolvingEctoplasmAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiImmuneTimer;
    bool   isImmune;

    void Reset()
    {
        m_creature->RemoveAllAuras();
        m_uiImmuneTimer = 0;
        isImmune = false;
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (!isImmune)
        {
            if (pSpell->School == SPELL_SCHOOL_FROST)
            {
//                m_creature->SetDisplayId(1751);
                DoCastSpellIfCan(m_creature, SPELL_IMMUNE_FROST, CAST_AURA_NOT_PRESENT);
                m_uiImmuneTimer = 10000;
                isImmune = true;
            }
            else if (pSpell->School == SPELL_SCHOOL_FIRE)
            {
//                m_creature->SetDisplayId(11138);
                DoCastSpellIfCan(m_creature, SPELL_IMMUNE_FIRE, CAST_AURA_NOT_PRESENT);
                m_uiImmuneTimer = 10000;
                isImmune = true;
            }
            else if (pSpell->School == SPELL_SCHOOL_NATURE)
            {
//                m_creature->SetDisplayId(4266);
                DoCastSpellIfCan(m_creature, SPELL_IMMUNE_NATURE, CAST_AURA_NOT_PRESENT);
                m_uiImmuneTimer = 10000;
                isImmune = true;
            }
            else if (pSpell->School == SPELL_SCHOOL_SHADOW)
            {
//                m_creature->SetDisplayId(767);
                DoCastSpellIfCan(m_creature, SPELL_IMMUNE_SHADOW, CAST_AURA_NOT_PRESENT);
                m_uiImmuneTimer = 10000;
                isImmune = true;
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiImmuneTimer < uiDiff)
        {
//            m_creature->SetDisplayId(1751);
            m_creature->RemoveAurasDueToSpell(SPELL_IMMUNE_SHADOW);
            m_creature->RemoveAurasDueToSpell(SPELL_IMMUNE_FROST);
            m_creature->RemoveAurasDueToSpell(SPELL_IMMUNE_FIRE);
            m_creature->RemoveAurasDueToSpell(SPELL_IMMUNE_NATURE);
            isImmune = false;
        }
        else
            m_uiImmuneTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_EvolvingEctoplasmAI(Creature* pCreature)
{
    return new EvolvingEctoplasmAI(pCreature);
}


void AddSC_wailing_caverns()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_evolving_ectoplasm";
    newscript->GetAI = &GetAI_EvolvingEctoplasmAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_disciple_of_naralex";
    newscript->GetAI = &GetAI_npc_disciple_of_naralex;
    newscript->pGossipHello =  &GossipHello_npc_disciple_of_naralex;
    newscript->pGossipSelect = &GossipSelect_npc_disciple_of_naralex;
    newscript->RegisterSelf();
}
