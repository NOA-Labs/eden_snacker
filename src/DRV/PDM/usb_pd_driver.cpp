/*
 * usb_pd_driver.c
 *
 * Created: 11/11/2017 23:55:12
 *  Author: jason
 */ 
#include <Arduino.h>

#include "usb_pd_driver.h"
#include "usb_pd.h"

#include "sc8721xx_drv.h"
#include "..\..\LIB\PUB\NOA_public.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(t) (sizeof(t) / sizeof(t[0]))
#endif

#ifdef CONFIG_COMMON_RUNTIME
#define CPRINTS(format, args...) cprints(CC_USBPD, format, ## args)
#define CPRINTF(format, args...) cprintf(CC_USBPD, format, ## args)
#else
#define CPRINTS(format, args...)  printf(format, ## args)
#define CPRINTF(format, args...)  printf(format, ## args)
#endif

extern struct tc_module tc_instance;
extern uint32_t g_us_timestamp_upper_32bit;
#ifdef NOA_PD_SNACKER
extern int ncp_bb_con1_en_pin;
#else
extern int ncp_bb_con1_en_pin;
extern int ncp_bb_con2_en_pin;
extern int ncp_bb_con3_en_pin;
#endif
// extern StructNCP81239RegisterMap g_stPMICData;
extern int pd_source_port_default_valtage;
extern int pd_source_port_default_current;

uint32_t pd_task_set_event(uint32_t event, int wait_for_reply)
{
	switch (event)
	{
		case PD_EVENT_TX:
			break;
		default:
			break;
	}
	return 0;
}

#if 0
const uint32_t pd_src_pdo[] = {
	PDO_FIXED(5000, 1500, PDO_FIXED_FLAGS),
};
const int pd_src_pdo_cnt = ARRAY_SIZE(pd_src_pdo);
#else
uint32_t pd_src_pdo[PDO_MAX_OBJECTS] = {
  PDO_FIXED(5000, 500, PDO_FIXED_FLAGS),
  PDO_FIXED(5000, 500, PDO_FIXED_FLAGS),
  PDO_FIXED(5000, 500, PDO_FIXED_FLAGS),
  PDO_FIXED(5000, 500, PDO_FIXED_FLAGS),
  PDO_FIXED(5000, 500, PDO_FIXED_FLAGS),
  PDO_FIXED(5000, 500, PDO_FIXED_FLAGS),
  PDO_FIXED(5000, 500, PDO_FIXED_FLAGS),
};
int pd_src_pdo_cnt = PDO_MAX_OBJECTS;
#endif

const uint32_t pd_snk_pdo[] = {
	PDO_FIXED(5000, 500, PDO_FIXED_FLAGS),
	PDO_FIXED(9000, 500, PDO_FIXED_FLAGS),
	PDO_FIXED(20000, 500, PDO_FIXED_FLAGS),
};
const int pd_snk_pdo_cnt = ARRAY_SIZE(pd_snk_pdo);

void pd_set_input_current_limit(int port, uint32_t max_ma,
	uint32_t supply_voltage)
{

}

int pd_is_valid_input_voltage(int mv)
{
	return 1;
}

int pd_snk_is_vbus_provided(int port)
{
	return 1;
}

timestamp_t get_time(void)
{
	timestamp_t t;
  t.val  = millis()*1000;
  t.val += micros()%1000;
  return t;
}

void pd_power_supply_reset(int port)
{
  if(port < 1 || port > 3) {  // support 1 - 3 port only
    return;
  }

  printf("Reset %d port Vbus Power\n", port);

	return;
}

void pd_power_supply_off(int port)
{
  printf("Off %d port Vbus Power\n", port);
  pd_source_port_default_valtage = 0;
  pd_source_port_default_current = 0;
#ifdef NOA_PD_SNACKER
  if(port != 1) {  // support 1 port only
    return;
  }
  sc87xx_drv_set_ce_pin(port, 0);
#else
  if(port < 1 || port > 3) {  // support 1 - 3 port only
    return;
  }
  switch (port) {
    case 1:
      digitalWrite(ncp_bb_con1_en_pin, LOW);
      break;
    case 2:
      ncp81239_pmic_set_tatus(port);
      digitalWrite(ncp_bb_con2_en_pin, LOW);
      break;
    case 3:
      digitalWrite(ncp_bb_con3_en_pin, LOW);
      break;
  }
#endif
//    delay(4);
  return;
}

int pd_custom_vdm(int port, int cnt, uint32_t *payload,
		  uint32_t **rpayload)
{
#if 0
	int cmd = PD_VDO_CMD(payload[0]);
	uint16_t dev_id = 0;
	int is_rw, is_latest;

	/* make sure we have some payload */
	if (cnt == 0)
		return 0;

	switch (cmd) {
	case VDO_CMD_VERSION:
		/* guarantee last byte of payload is null character */
		*(payload + cnt - 1) = 0;
		//CPRINTF("version: %s\n", (char *)(payload+1));
		break;
	case VDO_CMD_READ_INFO:
	case VDO_CMD_SEND_INFO:
		/* copy hash */
		if (cnt == 7) {
			dev_id = VDO_INFO_HW_DEV_ID(payload[6]);
			is_rw = VDO_INFO_IS_RW(payload[6]);

			is_latest = pd_dev_store_rw_hash(port,
							 dev_id,
							 payload + 1,
							 is_rw ?
							 SYSTEM_IMAGE_RW :
							 SYSTEM_IMAGE_RO);

			/*
			 * Send update host event unless our RW hash is
			 * already known to be the latest update RW.
			 */
			if (!is_rw || !is_latest)
				pd_send_host_event(PD_EVENT_UPDATE_DEVICE);

			//CPRINTF("DevId:%d.%d SW:%d RW:%d\n",
			//	HW_DEV_ID_MAJ(dev_id),
			//	HW_DEV_ID_MIN(dev_id),
			//	VDO_INFO_SW_DBG_VER(payload[6]),
			//	is_rw);
		} else if (cnt == 6) {
			/* really old devices don't have last byte */
			pd_dev_store_rw_hash(port, dev_id, payload + 1,
					     SYSTEM_IMAGE_UNKNOWN);
		}
		break;
	case VDO_CMD_CURRENT:
		CPRINTF("Current: %dmA\n", payload[1]);
		break;
	case VDO_CMD_FLIP:
		/* TODO: usb_mux_flip(port); */
		break;
#ifdef CONFIG_USB_PD_LOGGING
	case VDO_CMD_GET_LOG:
		pd_log_recv_vdm(port, cnt, payload);
		break;
#endif /* CONFIG_USB_PD_LOGGING */
	}
#endif // if 0

	return 0;
}

void pd_execute_data_swap(int port, int data_role)
{
	/* Do nothing */
}

int pd_check_data_swap(int port, int data_role)
{
	// Never allow data swap
	return 0;
}

int pd_check_power_swap(int port)
{
	/* Always refuse power swap */
	return 0;
}

int pd_board_checks(void)
{
	return EC_SUCCESS;
}

int pd_set_power_supply_ready(int port)
{
#if 0
	/* Disable charging */
	gpio_set_level(GPIO_USB_C0_CHARGE_L, 1);

	/* Enable VBUS source */
	gpio_set_level(GPIO_USB_C0_5V_EN, 1);

	/* notify host of power info change */
	pd_send_host_event(PD_EVENT_POWER_CHANGE);
#endif // if 0
#ifdef NOA_PD_SNACKER
//  CPRINTF("Enable %d Port Vbus Power", port);
  if(port != 1) {  // support 1 port only
    return EC_ERROR_UNKNOWN;
  }
  printf("power supply ready\n");
  sc87xx_drv_set_ce_pin(port, 1);
#else
  if (port == 2) {
    CPRINTF("Enable %d Port Vbus Power", port);
  }
  if(port < 1 || port > 3) {  // support 1 - 3 port only
    return EC_ERROR_UNKNOWN;
  }
  ncp81239_pmic_set_tatus(port);
  switch (port) {
    case 1:
      digitalWrite(ncp_bb_con1_en_pin, HIGH);
      break;
    case 2:
      digitalWrite(ncp_bb_con2_en_pin, HIGH);
      break;
    case 3:
      digitalWrite(ncp_bb_con3_en_pin, HIGH);
      break;
  }
#endif
	return EC_SUCCESS; /* we are ready */
}

void pd_transition_voltage(int port, int idx)
{
	/* No-operation: we are always 5V */
	
#if 0
	timestamp_t deadline;
	uint32_t mv = src_pdo_charge[idx - 1].mv;

	/* Is this a transition to a new voltage? */
	if (charge_port_is_active() && vbus[CHG].mv != mv) {
		/*
		 * Alter voltage limit on charge port, this should cause
		 * the port to select the desired PDO.
		 */
		pd_set_external_voltage_limit(CHG, mv);

		/* Wait for CHG transition */
		deadline.val = get_time().val + PD_T_PS_TRANSITION;
		CPRINTS("Waiting for CHG port transition");
		while (charge_port_is_active() &&
		       vbus[CHG].mv != mv &&
		       get_time().val < deadline.val)
			msleep(10);

		if (vbus[CHG].mv != mv) {
			CPRINTS("Missed CHG transition, resetting DUT");
			pd_power_supply_reset(DUT);
			return;
		}

		CPRINTS("CHG transitioned");
	}

	vbus[DUT].mv = vbus[CHG].mv;
	vbus[DUT].ma = vbus[CHG].ma;
#endif // if 0
#if 1 // mike SRC control only
//  CPRINTF("Transition %d Port voltage", port);
  if(port < 1 || port > 3) {  // support 1 - 3 port only
    return;
  }
  sc87xx_drv_set_output_vol(port, idx);
/*  if (pdo_mv != pdo_mv_source) {
    g_stPMICData[port].ucCR01DacTarget = pdo_mv + _TYPE_C_PMIC_VOLTAGE_OFFSET;
    pdo_mv_source = g_stPMICData[port].ucCR01DacTarget;
    ncp81239_pmic_set_voltage(port);
    delay(4);
  } */
#endif
}

void pd_check_dr_role(int port, int dr_role, int flags)
{
#if 0
	/* If UFP, try to switch to DFP */
	if ((flags & PD_FLAGS_PARTNER_DR_DATA) && dr_role == PD_ROLE_UFP)
	pd_request_data_swap(port);
#endif
}

void pd_check_pr_role(int port, int pr_role, int flags)
{
#if 0
	/*
	 * If partner is dual-role power and dualrole toggling is on, consider
	 * if a power swap is necessary.
	 */
	if ((flags & PD_FLAGS_PARTNER_DR_POWER) &&
	    pd_get_dual_role() == PD_DRP_TOGGLE_ON) {
		/*
		 * If we are a sink and partner is not externally powered, then
		 * swap to become a source. If we are source and partner is
		 * externally powered, swap to become a sink.
		 */
		int partner_extpower = flags & PD_FLAGS_PARTNER_EXTPOWER;

		if ((!partner_extpower && pr_role == PD_ROLE_SINK) ||
		     (partner_extpower && pr_role == PD_ROLE_SOURCE))
			pd_request_power_swap(port);
	}
#endif // if 0
}
