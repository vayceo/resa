#pragma once

inline uint16_t LastScriptingOpcode = 0;

enum {
    GAME_SCRIPT_IP_SIZE         = 255,
    GAME_SCRIPT_MAX_REFS        = 18,
    GAME_SCRIPT_MAX_PARAMS      = 16,
};

class ScriptIP_t {
public:
    ScriptIP_t() {
        memset(this, 0, sizeof(ScriptIP_t));
    }

    void Reset() {
        current_offset = 0;
    }

    uint8_t *GetIP() {
        return buffer;
    }

    template<typename T>
    void Write(T value) {
        memcpy(&buffer[current_offset], &value, sizeof(T));
        current_offset += sizeof(T);
        assert(current_offset < GAME_SCRIPT_IP_SIZE);
    }

    template<typename T>
    void Write(T *value, uint32_t size) {
        if (value && size) {
            memcpy(&buffer[current_offset], value, size);
            current_offset += size;
        }
    }

private:
    uint8_t buffer[GAME_SCRIPT_IP_SIZE];
    uint8_t current_offset;
};
VALIDATE_SIZE(ScriptIP_t, GAME_SCRIPT_IP_SIZE + 1);

class CRunningScript {
public:
    enum eParameter : uint8_t {
        PARAMETER_INTEGER       = 0x1,
        PARAMETER_FLOAT         = 0x6,
        PARAMETER_REF           = 0x3,
        PARAMETER_STRING        = 0xE,
        PARAMETER_EOP           = 0x0
    };

public:
    static inline ScriptIP_t        IP{};
    static inline uintptr           *Refs[GAME_SCRIPT_MAX_REFS];
    static inline uint16_t          ref_pos;

public:
    CRunningScript() {
        memset(this, 0, sizeof(CRunningScript));

        for (auto i = 0; i < GAME_SCRIPT_MAX_REFS; i++) {
            Refs[i] = nullptr;
        }

        ref_pos = 0;
    }

    void ResetForNewCommand() {
        IP.Reset();

        for (auto i = 0; i < GAME_SCRIPT_MAX_REFS; i++) {
            m_Refs[i] = 0;
        }

        ref_pos = 0;
    }

    void SetOpcode(uint16_t opcode) {
        LastScriptingOpcode = opcode;
        ResetForNewCommand();
        IP.Write(opcode);
    }

    void AddIntParam(int param) {
        IP.Write(PARAMETER_INTEGER);
        IP.Write(param);
    }

    void AddFloatParam(float param) {
        IP.Write(PARAMETER_FLOAT);
        IP.Write(param);
    }

    void AddRefParam(uintptr *ref) {
        IP.Write(PARAMETER_REF);

        Refs[ref_pos] = ref;
        m_Refs[ref_pos] = *ref;

        IP.Write(ref_pos);

        ref_pos++;
        assert(ref_pos < GAME_SCRIPT_MAX_REFS);
    }

    void AddStringParam(const char *str, uint8_t len) {
        IP.Write(PARAMETER_STRING);
        IP.Write(len);
        IP.Write(str, len);
    }

    void AddZeroTerminator() {
        IP.Write(PARAMETER_EOP);
    }

    auto ProcessCommand() {
        PCPointer = IP.GetIP();

        //  CRunningScript::ProcessOneCommand
        (( int8_t (*)(CRunningScript*))(g_libGTASA + (VER_x32 ? 0x0032B708 + 1 : 0x3F445C)))(this);
//        CallFunction<int8_t, GameScript_t *>(GAMEADDR(_text::_ZN14CRunningScript17ProcessOneCommandEv + 1), this);

        return m_CmpFlag;
    }

    void RetrieveRefs() {
        for (auto i = 0; i < ref_pos; i++) {
            *Refs[i] = m_Refs[i];
        }
    }

private:
    uintptr                 *pNext;
    uintptr                 *pPrevious;
    char                    ScriptName[8];
    uint8                   *BaseAddressOfThisScript;
    uint8                   *PCPointer;
    uint8                   *PCStack[8];
    uint16                  StackDepth;
    int32                   m_Refs[42];
    bool8 bActive;
    bool8 m_CmpFlag;
    bool8 IsThisAMissionScript;
    bool8 bIsThisAStreamedScript;
    bool8 bIsThisAMiniGameScript;
    int8 ScriptBrainType;
    uint32 ActivateTime;
    uint16 AndOrState;
    bool8 NotForLatestExpression;
    bool8 DeatharrestCheckEnabled;
    bool8 DoneDeatharrest;
    int32 EndOfScriptedCutscenePC;
    bool8 ThisMustBeTheOnlyMissionRunning;

//    [[maybe_unused]] uint8_t         _pad3[97];
//    [[maybe_unused]] uint8_t         m_CmpFlag;
//    [[maybe_unused]] uint8_t         _pad4[26];
};
VALIDATE_SIZE(CRunningScript, (VER_x32 ? 0x100 : 0x130));

struct ScriptCommand_t {
    uint16_t    Opcode;
    char        Params[GAME_SCRIPT_MAX_PARAMS];
};

int32_t ScriptCommand(const ScriptCommand_t *command, ...);

const ScriptCommand_t TASK_SHOOT_AT_COORD = { 0x0668, "ifffi" };
const ScriptCommand_t TASK_SHOOT_AT_CHAR = { 0x074D, "iii" };
const ScriptCommand_t TASK_CHAR_SLIDE_TO_COORD = { 0x07CD, "ifffff" };
const ScriptCommand_t TASK_USE_MOBILE_PHONE = { 0x0729, "ii" };
const ScriptCommand_t lock_camera_position = { 0x092F, "i" };
const ScriptCommand_t lock_camera_position1 = { 0x0930, "i" };
const ScriptCommand_t create_marker_icon = { 0x02A8, "fffiv" };
const ScriptCommand_t set_camera_pos_time_smooth = { 0x0936, "ffffffii" };
const ScriptCommand_t point_camera_transverse = { 0x0920, "ffffffii" };
const ScriptCommand_t restore_camera_to_user = { 0x0925, "" };
const ScriptCommand_t create_arrow_above_actor = { 0x0187, "iv" };
const ScriptCommand_t create_car = { 0x00A5, "ifffv" };	// (CAR_*|BIKE_*|BOAT_*), x, y, z, var_car
const ScriptCommand_t create_actor = { 0x009A, "iifffv" };	// PEDTYPE_*, #MODEL, x, y, z, var_actor
const ScriptCommand_t fade = { 0x016A, "ii" };		// (time in ms), FADE_*
const ScriptCommand_t set_current_time = { 0x00C0, "ii" };		// Hours, Minutes
const ScriptCommand_t create_player = { 0x0053, "vfffv" };	// 0, x, y, z, PLAYER_CHAR
const ScriptCommand_t create_actor_from_player = { 0x01F5, "vv" };		// PLAYER_CHAR, PLAYER_ACTOR
const ScriptCommand_t wait = { 0x0001, "i" };		// time_in_ms
const ScriptCommand_t destroy_car = { 0x00A6, "i" };		// var_car
const ScriptCommand_t set_car_z_angle = { 0x0175, "if" };		// var_car, angle
const ScriptCommand_t SET_CHAR_AMMO = { 0x017B, "iii" };	// var_actor, weapon, ammo
const ScriptCommand_t set_camera_behind_player = { 0x0373, "" };		// -/-
const ScriptCommand_t is_char_playing_anim = { 0x0611, "is" };
const ScriptCommand_t point_camera = { 0x0160, "fffi" };	// x, y, z, type
const ScriptCommand_t restore_camera_jumpcut = { 0x02EB, "" };		// -/-
const ScriptCommand_t set_camera_position = { 0x015F, "ffffff" }; // x, y, z, vx, vy, vz
const ScriptCommand_t tie_marker_to_car = { 0x0161, "iiiv" };	// var_car, ukn, ukn, var_marker
const ScriptCommand_t disable_marker = { 0x0164, "i" };		// var_marker
const ScriptCommand_t set_marker_color = { 0x0165, "ii" };		// var_marker, color
const ScriptCommand_t create_radar_marker_without_sphere = { 0x04CE, "fffiv" };	// x, y, z, marker_type, var_marker
const ScriptCommand_t show_on_radar = { 0x0168, "ii" };		// var_marker, size
const ScriptCommand_t create_icon_marker_sphere = { 0x02A7, "fffiv" };
const ScriptCommand_t is_actor_near_point_3d = { 0x00FE, "iffffffi" };
const ScriptCommand_t get_line_of_sight = { 0x06BD, "ffffffiiiii" }; // x1, y1, z1, x2, y2, z2, solid, vehicle, actor, obj, fx

const ScriptCommand_t get_ground_z = { 0x02ce, "fffv" };	// x, y, z, var_ground_z

//const ScriptCommand_t is_actor_near_point_3d		= {0x00FE,	"iffffffi"};
const ScriptCommand_t create_racing_checkpoint = { 0x06d5,	"ifffffffv" };	// type, x, y, z, nx, ny. nz, size, $id (n=next checkpoint)
/* 0       = Street Checkpoint with arrow to the next checkpoint
   1 and 2 = Street Checkpoint
   3 and 4 = Flight Checkpoint (circle) */
const ScriptCommand_t destroy_racing_checkpoint = { 0x06d6,	"i" };

// added by kyeman. (I don't use correct conventions so don't copy elsewhere
// without double checking they're how you want them, yalls.

const ScriptCommand_t restart_if_wasted_at = { 0x016C, "ffffi" };
const ScriptCommand_t toggle_player_controllable = { 0x01B4, "ii" }; // отключает элементы управления
const ScriptCommand_t set_actor_immunities = { 0x02ab, "iiiiii" };
const ScriptCommand_t set_car_immunities = { 0x02AC, "iiiiii" };
const ScriptCommand_t set_actor_can_be_decapitated = { 0x0446, "ii" };
const ScriptCommand_t create_object = { 0x0107, "ifffv" };
const ScriptCommand_t put_object_at = { 0x01Bc, "ifff" };
const ScriptCommand_t play_sound = { 0x018c, "fffi" };
const ScriptCommand_t TASK_ENTER_CAR_AS_DRIVER = { 0x05CB, "iii" };
const ScriptCommand_t send_actor_to_car_passenger = { 0x05CA, "iiii" };
const ScriptCommand_t TASK_LEAVE_ANY_CAR = { 0x0633, "i" };
const ScriptCommand_t get_car_z_angle = { 0x0174, "iv" };
const ScriptCommand_t destroy_train = { 0x07bd, "i" };
const ScriptCommand_t enable_train_traffic = { 0x06d7, "i" };
const ScriptCommand_t car_gas_tank_explosion = { 0x09C4, "ii" };
const ScriptCommand_t get_actor_z_angle = { 0x0172, "iv" };
const ScriptCommand_t set_actor_z_angle = { 0x0173, "if" };
const ScriptCommand_t lock_actor = { 0x04d7, "ii" };
//05BA
const ScriptCommand_t refresh_streaming_at = { 0x04E4, "ff" };
const ScriptCommand_t put_actor_in_car = { 0x036A, "ii" };
const ScriptCommand_t put_actor_in_car2 = { 0x0430, "iii" };

const ScriptCommand_t kill_actor = { 0x0321, "i" };

const ScriptCommand_t request_animation = { 0x04ED, "s" };
const ScriptCommand_t is_animation_loaded = { 0x04EE, "s" };
//0812
const ScriptCommand_t TASK_CHAR_ARREST_CHAR = { 0x0850, "ii" };
const ScriptCommand_t apply_animation = { 0x0812, "issfiiiii" };
const ScriptCommand_t set_actor_weapon_droppable = { 0x087e, "ii" };
const ScriptCommand_t set_actor_money = { 0x03fe, "ii" };
const ScriptCommand_t text_clear_all = { 0x00be, "" };
const ScriptCommand_t set_car_hydraulics = { 0x07FF,	"ii" }; //iCar, i (0=off/1=on)

const ScriptCommand_t attach_object_to_actor = { 0x069b, "iiffffff" };

const ScriptCommand_t create_pickup = { 0x0213, "iifffv" };
const ScriptCommand_t destroy_pickup = { 0x0215, "i" };

const ScriptCommand_t change_car_skin = { 0x06ED,	"ii" };
const ScriptCommand_t add_car_component = { 0x06E7, "iiv" };	// CAR, COMPONENT, COMPONENT VAR NAME
const ScriptCommand_t remove_component = { 0x06E8, "ii" };

const ScriptCommand_t show_on_radar2 = { 0x018b, "ii" };

const ScriptCommand_t attach_object_to_car = { 0x0681, "iiffffff" };
const ScriptCommand_t is_object_attached = { 0x0685, "i" };
const ScriptCommand_t set_char_never_targeted = { 0x0568, "ii" };
const ScriptCommand_t clear_char_tasks = { 0x0792, "i" };
const ScriptCommand_t change_stat = { 0x0629, "ii" };
const ScriptCommand_t set_char_weapon_skill = { 0x081A, "ii" };

const ScriptCommand_t toggle_car_tires_vulnerable = { 0x053f, "ii" };
const ScriptCommand_t set_fighting_style = { 0x07fe, "iii" };

const ScriptCommand_t destroy_object = { 0x0108, "i" };
const ScriptCommand_t create_radar_marker_icon = { 0x0570, "fffii" };
const ScriptCommand_t put_trailer_on_cab = { 0x893, "ii" };
const ScriptCommand_t detach_trailer_from_cab = { 0x7AC, "ii" };
const ScriptCommand_t create_explosion_with_radius = { 0x0948, "fffii" };
const ScriptCommand_t enable_zone_names = { 0x09BA, "i" };

const ScriptCommand_t destroy_particle = { 0x650, "i" };
//opcode_066a('PETROLCAN', lhActor47, 0.0, 0.116, 0.048, lf05f, lf060, lf061, 1, l0089);
const ScriptCommand_t attach_particle_to_actor = { 0x0669, "sifffiv" };
const ScriptCommand_t make_particle_visible = { 0x64c, "i" };

const ScriptCommand_t create_property_pickup = { 0x518, "i" };

const ScriptCommand_t SET_PLAYER_DRUNKENNESS = { 0x052C, "ii" }; // player, severity (0-255)
const ScriptCommand_t TASK_LOOK_AT_COORD = { 0x06A9, "ifffi" };

//opcode_066a('PETROLCAN', lhActor47, 0.0, 0.116, 0.048, lf05f, lf060, lf061, 1, l0089);
const ScriptCommand_t attach_particle_to_actor2			= { 0x066a, "siffffffiv" };